#pragma once
#include <string>
#include "Date.h"
using namespace std;
class CExpenseItem
{
	wstring m_strDescription;
	wstring  m_strType;
	int m_nCost;
	Date m_AddDate;
public:
	CExpenseItem(WCHAR * _lpzstrDescription, WCHAR * _lpzstrType, const int & _nCost, const Date & _AddDate);
	virtual ~CExpenseItem();

	WCHAR * GetDescription() const;
	WCHAR * GetCategory() const;
	int GetCost() const;
	WCHAR * GetCostWStr() const;
	Date GetDate() const;
};
