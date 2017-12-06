/*---------------------------------------
| Student ID : 1412657					|
| Full Name : Nguyễn Tường Vy    		|
-----------------------------------------

Environment: 	Windows 10
Tool : Visual Studio 2017
*/

#include "stdafx.h"
#include "PerFinance.h"
#include "ExpenseItem.h"
#include <commctrl.h>
#include <windowsX.h>
#include <vector>
#include <fstream>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

using namespace std;

#define MAX_LOADSTRING 256
int seperator = 10;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int MAXSPENDLIST = 10;
HWND gbox_AddExpense, lb_AddExpense_Category, cbox_AddExpense_Category, lb_AddExpense_Description, tb_AddExpense_Description, lb_AddExpense_Cost, tb_AddExpense_Cost, btn_AddExpense_Add,
gbox_Expenses, btn_Expenses_Edit, btn_Expenses_Remove, lv_Expenses,
gbox_Statistic, lb_totalCost, tb_totalCost;
vector<HWND> lb_Explanation;
vector<LPWSTR> categoryString;
unsigned int winSizeX = 600,
winSizeY = 700,
minWindowWidth = 480;
int isEditting = 0;
WCHAR szDefaultFileNameBuffer[MAX_LOADSTRING + 20];
double *percentArray = nullptr;
int colNum = 0;

int ImportData(LPWCH fileName, bool removeOlddata);
double * Statistic();
double CalculateTotalCost();

vector<CExpenseItem> g_expenseVector;
DWORD color[24] = { 0x800000,
					0xB8860B,
					0x6B8E23,
					0x008B8B,
					0x4682B4,
					0x00008B,
					0x8B008B,
					0x9932CC,
					0xC71585,
					0x000000,
					0xFF0000,
					0xFFA500,
					0x7FFFD4,
					0xFFFF00,
					0x00FF00,
					0x00FF00,
					0x1E90FF,
					0x0000FF,
					0xFF00FF,
					0xFF69B4,
					0xB0C4DE,
					0xA9A9A9,
					0xFFFFFF,
					0xFFE4B5 };

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int ShowExpenseItem(CExpenseItem _item, int _oder);
int ExportData(LPWCH fileName);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PERFINANCE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PERFINANCE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PERFINANCE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_PERFINANCE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"Quản lý chi tiêu", (WS_OVERLAPPEDWINDOW) /*^ (WS_THICKFRAME | WS_MAXIMIZEBOX)*/,
		0, 0, winSizeX, winSizeY, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);


	return TRUE;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnSize(HWND hwnd, UINT state, int cx, int cy);
void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
int FetchItem(unsigned int index);

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_GETMINMAXINFO, OnGetMinMaxInfo);
		
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		HBRUSH hbrush, hbrOld;
		HPEN hpen, hpenOld;

		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);
		int width = windowRect.right - windowRect.left,
			height = windowRect.bottom - windowRect.top;

		int colNum = width / ((minWindowWidth - 4 * seperator) / 2),
			rowNum = (lb_Explanation.size() + colNum - 1) / colNum;

		int left = 2 * seperator,
			top = height - (76 + rowNum * 20) - 2 * seperator,
			right = width - 4 * seperator,
			bottom = top + 25;


		int width_statBar = right - left;
		left = 2 * seperator;

		hbrush = CreateSolidBrush(color[1]);
		hbrOld = (HBRUSH)SelectObject(hdc, hbrush);
		hpen = CreatePen(PS_SOLID, 0, color[1]);
		hpenOld = (HPEN)SelectObject(hdc, hpen);

		for (unsigned int i = 0; i < lb_Explanation.size(); ++i)
		{

			//
			right = int(left + percentArray[i] * width_statBar);

			// retangles of expalanation
			Rectangle(hdc, 2 * seperator + (i % colNum)*(minWindowWidth - 4 * seperator) / 2, bottom + 10 + (i / colNum) * 20, 2 * seperator + (i % colNum)*(minWindowWidth - 4 * seperator) / 2 + 15, bottom + 10 + (i / colNum) * 20 + 15);

			Rectangle(hdc, left, top, right, bottom);
			left = right;
			DeleteObject(hbrush);
			DeleteObject(hpen);

			hbrush = CreateSolidBrush(color[i + 2]);
			SelectObject(hdc, hbrush);
			hpen = CreatePen(PS_SOLID, 0, color[i + 2]);
			SelectObject(hdc, hpen);
		}
		SelectObject(hdc, hbrOld);
		SelectObject(hdc, hpenOld);
		DeleteObject(hbrush);
		DeleteObject(hpen);


		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		ExportData(szDefaultFileNameBuffer);
		//for (unsigned int i = 0; i < categoryString.size(); ++i)
		//{
		//	delete[] categoryString[i];
		//	categoryString[i] = nullptr;
		//}
		delete[] percentArray;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HFONT hFontLarge = CreateFont(lf.lfHeight - 7, lf.lfWidth - 7,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);


	GetCurrentDirectory(MAX_LOADSTRING, szDefaultFileNameBuffer);
	wsprintf(szDefaultFileNameBuffer, L"%ls\\data", szDefaultFileNameBuffer);


	// groupbox "Thêm chi tiêu"
	gbox_AddExpense = CreateWindow(WC_BUTTON, L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		seperator, seperator, 0, 0, hWnd, 0, hInst, 0);
	SendMessage(gbox_AddExpense, WM_SETFONT, (WPARAM)hFont, TRUE);
	//cửa số con của groupbox  "Thêm chi tiêu" 
	{
		//Loại chi tiêu
		{
			lb_AddExpense_Category = CreateWindow(WC_STATIC, L"Loại chi tiêu", WS_CHILD | WS_VISIBLE, seperator, 3 * seperator, 80, 14, gbox_AddExpense, 0, hInst, 0);
			SendMessage(lb_AddExpense_Category, WM_SETFONT, (WPARAM)hFont, TRUE);

			cbox_AddExpense_Category = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
				seperator, 3 * seperator + 15, 0, 0, gbox_AddExpense, 0, 0, 0);
			SendMessage(cbox_AddExpense_Category, WM_SETFONT, (WPARAM)hFont, TRUE);

		}

		//Nội dung
		{
			lb_AddExpense_Description = CreateWindow(WC_STATIC, L"Nội dung", WS_CHILD | WS_VISIBLE, 0, 0, 60, 14, gbox_AddExpense, 0, hInst, 0);
			SendMessage(lb_AddExpense_Description, WM_SETFONT, (WPARAM)hFont, TRUE);

			tb_AddExpense_Description = CreateWindow(WC_EDIT, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
				0, 0, 0, 0, gbox_AddExpense, 0, 0, 0);
			SendMessage(tb_AddExpense_Description, WM_SETFONT, (WPARAM)hFont, TRUE);
		}

		//Số tiền
		{
			lb_AddExpense_Cost = CreateWindow(WC_STATIC, L"Số tiền", WS_CHILD | WS_VISIBLE, 0, 0, 40, 14, gbox_AddExpense, 0, hInst, 0);
			SendMessage(lb_AddExpense_Cost, WM_SETFONT, (WPARAM)hFont, TRUE);

			tb_AddExpense_Cost = CreateWindow(WC_EDIT, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER,
				0, 0, 0, 0, gbox_AddExpense, 0, 0, 0);
			SendMessage(tb_AddExpense_Cost, WM_SETFONT, (WPARAM)hFont, TRUE);
		}

		// button Thêm
		btn_AddExpense_Add = CreateWindow(WC_BUTTON, L"Thêm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 60, 21, hWnd, (HMENU)IDC_BTN_ADDSPEND_ADD, hInst, 0);
		SendMessage(btn_AddExpense_Add, WM_SETFONT, (WPARAM)hFont, TRUE);
	}


	double totalCost = 0;
	// groupbox "Chi tiết chi tiêu"
	gbox_Expenses = CreateWindow(WC_BUTTON, L"Toàn bộ danh sách các chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		seperator, 85 + 2 * seperator, 0, 0, hWnd, 0, hInst, 0);
	SendMessage(gbox_Expenses, WM_SETFONT, (WPARAM)hFont, TRUE);

	btn_Expenses_Edit = CreateWindow(WC_BUTTON, L"Sửa", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		2 * seperator, 85 + 3 * seperator + seperator + 10, 60, 21, hWnd, (HMENU)IDC_BTN_SPENDS_EDIT, hInst, 0);
	SendMessage(btn_Expenses_Edit, WM_SETFONT, (WPARAM)hFont, TRUE);

	btn_Expenses_Remove = CreateWindow(WC_BUTTON, L"Xóa", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		seperator + seperator + 60 + seperator, 85 + 3 * seperator + seperator + 10, 60, 21, hWnd, (HMENU)IDC_BTN_SPENDS_REMOVE, hInst, 0);
	SendMessage(btn_Expenses_Remove, WM_SETFONT, (WPARAM)hFont, TRUE);

	//cửa số con của groupbos "Chi tiết chi tiêu"
	{
		lv_Expenses = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT | WS_VSCROLL | WS_HSCROLL,
			seperator, seperator + 10 + 21 + 2 * seperator, 0, 0, gbox_Expenses, 0, hInst, 0);
		ListView_SetExtendedListViewStyle(lv_Expenses, LVS_EX_FULLROWSELECT);
		SendMessage(lv_Expenses, WM_SETFONT, (WPARAM)hFont, TRUE);

		//các cột
		LVCOLUMN lvCol;
		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 250;
		lvCol.pszText = L"Nội dung";
		ListView_InsertColumn(lv_Expenses, 0, &lvCol);

		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 104;
		lvCol.pszText = L"Loại chi tiêu";
		ListView_InsertColumn(lv_Expenses, 1, &lvCol);

		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.cx = 90;
		lvCol.pszText = L"Số tiền";
		ListView_InsertColumn(lv_Expenses, 2, &lvCol);

		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.cx = 100;
		lvCol.pszText = L"Ngày thêm";
		ListView_InsertColumn(lv_Expenses, 3, &lvCol);

		ImportData(szDefaultFileNameBuffer, false);
		totalCost = CalculateTotalCost();

		for (unsigned int i = 0; i < g_expenseVector.size(); ++i)
		{
			ShowExpenseItem(g_expenseVector[i], i);
		}
	}

	// groupbox "Thống kê"
	const int BUFFERSIZE = 256;
	WCHAR buffer[BUFFERSIZE],
		curPath[BUFFERSIZE],
		key[5];
	swprintf_s(buffer, BUFFERSIZE, L"Tổng tiền : %.0lf đồng", totalCost);
	gbox_Statistic = CreateWindow(WC_BUTTON, L"Thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		0, 0, 0, 0, hWnd, 0, hInst, 0);
	SendMessage(gbox_Statistic, WM_SETFONT, (WPARAM)hFont, TRUE);

	lb_totalCost = CreateWindow(WC_STATIC, buffer, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, gbox_Statistic, 0, hInst, 0);
	SendMessage(lb_totalCost, WM_SETFONT, (WPARAM)hFontLarge, TRUE);

	int componentNum = ComboBox_GetCount(cbox_AddExpense_Category);
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	int width = windowRect.right - windowRect.left, height = windowRect.bottom - windowRect.top;
	int left = seperator,
		top = 46 + 5 * seperator,
		right = width - 3 * seperator,
		bottom = top + 26 - (height - (160 + 2 * seperator) - 4 * seperator);

	//load loại chi tiêu
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wcscat_s(curPath, BUFFERSIZE, L"\\confg.ini");
	int i = 0;
	do
	{
		swprintf_s(key, L"%d", i);
		GetPrivateProfileString(L"Loại chi tiêu", key, 0, buffer, BUFFERSIZE, curPath);
		if (buffer[0] != L'\0')
		{
			ComboBox_AddString(cbox_AddExpense_Category, (LPARAM)buffer);
		}
		else
			break;
		categoryString.push_back(_wcsdup(buffer));
		if (i == MAXSPENDLIST)
			break;
		++i;

	} while (buffer[0] != L'\0');
	bool hasOtherCategory = false;
	for (unsigned int i = 0; i < categoryString.size(); ++i)
	{
		if (wcscmp(categoryString[i], L"Khác") == 0 || wcscmp(categoryString[i], L"khác") == 0)
		{
			hasOtherCategory = true;
			break;
		}
	}

	//thêm loại chi tiêu "Khác"
	if (!hasOtherCategory)
	{
		ComboBox_AddString(cbox_AddExpense_Category, (LPARAM)L"Khác");
		categoryString.push_back(L"Khác");
	}

	percentArray = new double[categoryString.size() + 1];
	Statistic();
	for (unsigned int i = 0; i < categoryString.size(); ++i)
	{
		swprintf_s(buffer, MAX_LOADSTRING, L"%ls (%.2lf%%)", categoryString[i], percentArray[i] * 100);
		HWND hwnd = CreateWindow(WC_STATIC, buffer, WS_CHILD | WS_VISIBLE, left + (i % 2)*(minWindowWidth - 4 * seperator) / 2 + 20, top + (i / 2) * 20, 180, 14, gbox_Statistic, 0, hInst, 0);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
		lb_Explanation.push_back(hwnd);
	}
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BTN_ADDSPEND_ADD:
	{
		if (!isEditting)
		{
			int i = ComboBox_GetCurSel(cbox_AddExpense_Category);
			if (i < 0 || GetWindowTextLength(tb_AddExpense_Description) == 0 || GetWindowTextLength(tb_AddExpense_Cost) == 0)
				break;
			WCHAR buffer_description[256];
			GetWindowText(tb_AddExpense_Description, buffer_description, 256);
			WCHAR buffer_Category[256];
			GetWindowText(cbox_AddExpense_Category, buffer_Category, 256);
			WCHAR buffer_Cost[256];
			GetWindowText(tb_AddExpense_Cost, buffer_Cost, 256);
			WCHAR * tmp = buffer_Cost + wcslen(buffer_Cost);
			g_expenseVector.push_back(CExpenseItem((WCHAR*)buffer_description, (WCHAR*)buffer_Category, wcstol(buffer_Cost, &tmp, 10), Date()));
			ShowExpenseItem(g_expenseVector[g_expenseVector.size() - 1], ListView_GetItemCount(lv_Expenses));
			SetWindowText(btn_AddExpense_Add, L"Thêm");
			SetWindowText(tb_AddExpense_Description, 0);
			SetWindowText(tb_AddExpense_Cost, 0);
			ComboBox_SetCurSel(cbox_AddExpense_Category, -1);
		}
		else
		{
			int i = ComboBox_GetCurSel(cbox_AddExpense_Category);
			if (i < 0 || GetWindowTextLength(tb_AddExpense_Description) == 0 || GetWindowTextLength(tb_AddExpense_Cost) == 0)
				break;
			WCHAR buffer_description[256];
			GetWindowText(tb_AddExpense_Description, buffer_description, 256);
			WCHAR buffer_Category[256];
			GetWindowText(cbox_AddExpense_Category, buffer_Category, 256);
			WCHAR buffer_Cost[256];
			GetWindowText(tb_AddExpense_Cost, buffer_Cost, 256);
			WCHAR * tmp = buffer_Cost + wcslen(buffer_Cost);
			CExpenseItem newItem = CExpenseItem((WCHAR*)buffer_description, (WCHAR*)buffer_Category, wcstol(buffer_Cost, &tmp, 10), g_expenseVector[isEditting - 1].GetDate());
			g_expenseVector[isEditting - 1] = newItem;

			ListView_DeleteItem(lv_Expenses, isEditting - 1);
			ShowExpenseItem(newItem, isEditting - 1);
			isEditting = 0;
			SetWindowText(btn_AddExpense_Add, L"Thêm");
			SetWindowText(tb_AddExpense_Description, 0);
			SetWindowText(tb_AddExpense_Cost, 0);
			ComboBox_SetCurSel(cbox_AddExpense_Category, -1);
		}
		const int BUFFERSIZE = 256;
		WCHAR buffer[BUFFERSIZE];
		double totalCost = CalculateTotalCost();
		swprintf_s(buffer, BUFFERSIZE, L"Tổng tiền: %.0lf đồng", totalCost);
		SetWindowText(lb_totalCost, buffer);


		percentArray = Statistic();
		WCHAR explanationBuffer[MAX_LOADSTRING];
		for (unsigned int i = 0; i < lb_Explanation.size(); ++i)
		{
			swprintf_s(explanationBuffer, MAX_LOADSTRING, L"%ls (%.2lf%%)", categoryString[i], percentArray[i] * 100);
			SetWindowText(lb_Explanation[i], explanationBuffer);
		}
		InvalidateRect(gbox_Statistic, 0, TRUE);
		break;
	}
	case IDC_BTN_SPENDS_EDIT:
	{
		if (ListView_GetSelectedCount(lv_Expenses) == 0)
			break;
		LRESULT index = SendMessage(lv_Expenses, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)LVNI_SELECTED);

		FetchItem(index);

		break;
	}
	case IDC_BTN_SPENDS_REMOVE:
	{
		if (ListView_GetSelectedCount(lv_Expenses) == 0)
			break;
		LRESULT index = SendMessage(lv_Expenses, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)LVNI_SELECTED);
		ListView_DeleteItem(lv_Expenses, index);
		g_expenseVector.erase(g_expenseVector.begin() + index);

		//recalculate total cost
		const int BUFFERSIZE = 256;
		WCHAR buffer[BUFFERSIZE];
		double totalCost = CalculateTotalCost();
		swprintf_s(buffer, BUFFERSIZE, L"Tổng tiền: %.0lf", totalCost);
		SetWindowText(lb_totalCost, buffer);

		percentArray = Statistic();
		WCHAR explanationBuffer[MAX_LOADSTRING];
		for (unsigned int i = 0; i < lb_Explanation.size(); ++i)
		{
			swprintf_s(explanationBuffer, MAX_LOADSTRING, L"%ls (%.2lf%%)", categoryString[i], percentArray[i] * 100);
			SetWindowText(lb_Explanation[i], explanationBuffer);
		}

		InvalidateRect(gbox_Statistic, NULL, TRUE);

		break;
	}
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		ExportData(szDefaultFileNameBuffer);
		DestroyWindow(hWnd);
		break;
	}

}

void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	SetWindowPos(gbox_AddExpense, nullptr, 0, 0, cx - 2 * seperator, 85, SWP_NOMOVE);

	int width_Category = int(cx * 0.18);
	if (width_Category > 200)
		width_Category = 200;
	SetWindowPos(cbox_AddExpense_Category, nullptr, 0, 0, width_Category, 21, SWP_NOMOVE);

	int width_Cost = cx - (2 * seperator + (width_Category + 2 * seperator) + (int(cx * 0.34) + 2 * seperator) + 2 * seperator) - (60 + 2 * seperator);
	if (width_Cost > 220)
		width_Cost = 220;
	int width_Description = cx - (seperator + width_Category + 2 * seperator + width_Cost + 2 * seperator + 2 * seperator) - (60 + 3 * seperator);

	SetWindowPos(lb_AddExpense_Cost, nullptr, seperator + width_Category + 2 * seperator + width_Description + 2 * seperator, seperator + 20, 0, 0, SWP_NOSIZE);
	SetWindowPos(tb_AddExpense_Cost, nullptr, seperator + width_Category + 2 * seperator + width_Description + 2 * seperator, 3 * seperator + 16, width_Cost, 19, 0);

	SetWindowPos(lb_AddExpense_Description, nullptr, seperator + width_Category + 2 * seperator, 3 * seperator, 0, 0, SWP_NOSIZE);
	SetWindowPos(tb_AddExpense_Description, nullptr, seperator + width_Category + 2 * seperator, 3 * seperator + 16, width_Description, 19, 0);

	SetWindowPos(btn_AddExpense_Add, nullptr, cx - 2 * seperator - 60, 4 * seperator + 15, 0, 0, SWP_NOSIZE);


	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	int width = windowRect.right - windowRect.left, height = windowRect.bottom - windowRect.top;
	int newColNum = width / ((minWindowWidth - 4 * seperator) / 2),
		newRowNum = (ComboBox_GetCount(cbox_AddExpense_Category) + newColNum - 1) / newColNum;
	int height_gbox_Statistic = 71 + 3 * seperator + newRowNum * 20;
	if (newColNum != colNum)
	{
		int left = seperator,
			top = height_gbox_Statistic - newRowNum * 20 - seperator,
			colSize = (minWindowWidth - 4 * seperator) / 2;
		for (unsigned int i = 0; i < lb_Explanation.size(); ++i)
		{
			SetWindowPos(lb_Explanation[i], 0, left + (i % newColNum)*colSize + 20, top + (i / newColNum) * 20, 0, 0, SWP_NOSIZE);
		}
		colNum = newColNum;
	}

	SetWindowPos(gbox_Expenses, nullptr, 0, 0, cx - 2 * seperator, cy - (85 + 2 * seperator) - (height_gbox_Statistic + 2 * seperator), SWP_NOMOVE);
	SetWindowPos(lv_Expenses, nullptr, 0, 0, cx - 4 * seperator, cy - (85 + 2 * seperator) - (height_gbox_Statistic + 2 * seperator) - (3 * seperator + 21 + 2 * seperator), SWP_NOMOVE);
	ListView_SetColumnWidth(lv_Expenses, 0, int(0.38 *(cx - 4 * seperator)) - 14);
	ListView_SetColumnWidth(lv_Expenses, 1, int(0.22 *(cx - 4 * seperator)));
	ListView_SetColumnWidth(lv_Expenses, 2, int(0.22 *(cx - 4 * seperator)));
	ListView_SetColumnWidth(lv_Expenses, 3, int(0.18 *(cx - 4 * seperator)));


	SetWindowPos(gbox_Statistic, nullptr, seperator, cy - (height_gbox_Statistic)-seperator, cx - 2 * seperator, height_gbox_Statistic, 0);
	SetWindowPos(lb_totalCost, nullptr, int((cx - 4 * seperator) / 2 - 80), 2 * seperator, 180, 25, 0);


}

void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	lpMinMaxInfo->ptMinTrackSize.x = minWindowWidth;
	lpMinMaxInfo->ptMinTrackSize.y = 550;
	//lpMinMaxInfo->ptMaxTrackSize.x = 1000;

}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int ShowExpenseItem(CExpenseItem _item, int _oder)
{
	LVITEM insrtItem;
	// 1st column
	insrtItem.mask = LVIF_TEXT;
	insrtItem.iSubItem = 0;
	insrtItem.iItem = _oder;
	//insrtItem.pszText = new WCHAR[256];
	insrtItem.pszText = _item.GetDescription();
	insrtItem.cchTextMax = wcslen(insrtItem.pszText);
	ListView_InsertItem(lv_Expenses, &insrtItem);

	LPWSTR lpwstr;
	// 2nd column
	lpwstr = _item.GetCategory();
	ListView_SetItemText(lv_Expenses, _oder, 1, lpwstr);
	delete lpwstr;

	// 3rd column
	lpwstr = _item.GetCostWStr();
	ListView_SetItemText(lv_Expenses, _oder, 2, lpwstr);
	delete lpwstr;

	// 4th column
	lpwstr = _wcsdup(_item.GetDate().ToWString().c_str());
	ListView_SetItemText(lv_Expenses, _oder, 3, lpwstr);
	delete lpwstr;

	return 0;
}
//load selected item in listview to the editting zone
int FetchItem(unsigned int index)
{
	//load loại chi tiêu
	int categoryIndex = ComboBox_FindString(cbox_AddExpense_Category, 0, g_expenseVector[index].GetCategory());

	if (categoryIndex < 0)//in case category of item is not a built-in category => regarding as "other"/"Other" category
	{
		int indexOfOther = ComboBox_FindString(cbox_AddExpense_Category, 0, L"Khác");
		if (indexOfOther < 0)
			indexOfOther = ComboBox_FindString(cbox_AddExpense_Category, 0, L"Khác");
		else//there is no other category => add one to the combobox
		{
			ComboBox_AddString(cbox_AddExpense_Category, (LPARAM)L"Khác");
			categoryString.push_back(L"Khác");
			indexOfOther = ComboBox_FindString(cbox_AddExpense_Category, 0, L"Khác");
		}


		ComboBox_SetCurSel(cbox_AddExpense_Category, indexOfOther);
	}
	else
		ComboBox_SetCurSel(cbox_AddExpense_Category, categoryIndex);

	LPWSTR wstr;
	wstr = g_expenseVector[index].GetDescription();
	SetWindowText(tb_AddExpense_Description, wstr);
	delete[] wstr;

	wstr = g_expenseVector[index].GetCostWStr();
	SetWindowText(tb_AddExpense_Cost, g_expenseVector[index].GetCostWStr());
	delete[] wstr;

	SetWindowText(btn_AddExpense_Add, L"Lưu");

	isEditting = index + 1;//save index for later using, this var show that app is in editting mode and the position (starts from 1) of editting item
	return 0;
}

int ExportData(LPWCH fileName)
{
	int length = wcslen(fileName);
	if (!(fileName[length - 3] == 's' && fileName[length - 2] == 'd' && fileName[length - 1] == 't'))//add extension
	{
		fileName[length] = '.';
		fileName[length + 1] = 's';
		fileName[length + 2] = 'd';
		fileName[length + 3] = 't';
		fileName[length + 4] = '\0';
	}
	ofstream dataFile(fileName, ios::out | ios::binary);

	if (!dataFile)
		return -1;
	size_t expenseNum = g_expenseVector.size();
	dataFile.write((char*)(&expenseNum), sizeof(expenseNum));

	length;
	WCHAR * wcstr;
	int cost;
	Date date;

	for (unsigned int i = 0; i < expenseNum; ++i)
	{
		//lưu Loại chi tiêu
		wcstr = g_expenseVector[i].GetCategory();
		length = wcslen(wcstr);
		dataFile.write((char*)(&length), sizeof(length));
		for (int j = 0; j <= length; ++j)//include saving the terminated character for easier later loading
		{
			dataFile.write((char*)(wcstr + j), sizeof(wcstr[0]));
		}
		delete[] wcstr;

		//Lưu Nội dung
		wcstr = g_expenseVector[i].GetDescription();
		length = wcslen(wcstr);
		dataFile.write((char*)(&length), sizeof(length));
		for (int j = 0; j <= length; ++j)//include saving the terminated character for easier later loading
		{
			dataFile.write((char*)(wcstr + j), sizeof(wcstr[0]));
		}
		delete[] wcstr;

		//lưu Tổng tiền
		cost = g_expenseVector[i].GetCost();
		dataFile.write((char*)(&cost), sizeof(cost));

		//Lưu Ngày
		date = g_expenseVector[i].GetDate();
		dataFile.write((char*)(&date), sizeof(date));
	}
	return 0;
}


int ImportData(LPWCH fileName, bool removeOlddata)
{
	ifstream dataFile(fileName, ios::in | ios::binary);

	if (!dataFile)
		return -1;
	int expenseNum;
	dataFile.read((char*)(&expenseNum), sizeof(expenseNum));
	if (dataFile.good() == false)
		return -2;
	if (expenseNum < 0)
		return -3;

	if (removeOlddata)
	{
		g_expenseVector.clear();
	}
	int length;
	WCHAR *wcstrCategory, *wcstrDescription;
	int cost;
	Date date;
	CExpenseItem * item;
	for (int i = 0; i < expenseNum; ++i)
	{
		dataFile.read((char*)(&length), sizeof(length));
		wcstrCategory = new WCHAR[length + 1];
		for (int j = 0; j <= length; ++j)//include loading the terminated character
		{
			dataFile.read((char*)(wcstrCategory + j), sizeof(wcstrCategory[0]));
		}

		dataFile.read((char*)(&length), sizeof(length));
		wcstrDescription = new WCHAR[length + 1];
		for (int j = 0; j <= length; ++j)//include loading the terminated character
		{
			dataFile.read((char*)(wcstrDescription + j), sizeof(wcstrDescription[0]));
		}

		dataFile.read((char*)(&cost), sizeof(cost));

		dataFile.read((char*)(&date), sizeof(date));
		item = new CExpenseItem(wcstrDescription, wcstrCategory, cost, date);
		g_expenseVector.push_back(*item);

		delete[] wcstrCategory;
		delete[] wcstrDescription;
		delete item;
	}
	return 0;
}

double * Statistic()
{
	int componentNum = ComboBox_GetCount(cbox_AddExpense_Category);
	for (int i = 0; i <= componentNum; ++i)
		percentArray[i] = 0;
	for (unsigned int i = 0; i < g_expenseVector.size(); ++i)
	{
		int categoryIndex = ComboBox_FindString(cbox_AddExpense_Category, 0, g_expenseVector[i].GetCategory());
		if (categoryIndex < 0) // loại chi tiêu khác
		{
			int indexOfOther = ComboBox_FindString(cbox_AddExpense_Category, 0, L"Khác");
			if (indexOfOther < 0)
				indexOfOther = ComboBox_FindString(cbox_AddExpense_Category, 0, L"Khác");
			else//ko phải là 1 loại chi tiêu khác  => thêm 1 combobox
			{
				ComboBox_AddString(cbox_AddExpense_Category, (LPARAM)L"Khác");
				categoryString.push_back(L"Kh");
				indexOfOther = ComboBox_FindString(cbox_AddExpense_Category, 0, L"Khác");
				++componentNum;
			}

			percentArray[indexOfOther] += g_expenseVector[i].GetCost();
		}
		else
			percentArray[categoryIndex] += g_expenseVector[i].GetCost();
		percentArray[componentNum] += g_expenseVector[i].GetCost();
	}
	if (percentArray[componentNum] != 0)//chặn việc tổng tiền bằng 0
		for (int i = 0; i < componentNum; ++i)
		{
			percentArray[i] /= percentArray[componentNum];
		}
	return percentArray;
}

double CalculateTotalCost()
{
	double totalCost = 0;
	for (unsigned int i = 0; i < g_expenseVector.size(); ++i)
		totalCost += g_expenseVector[i].GetCost();
	return totalCost;
}