#include "stdafx.h"
#include "ExpenseItem.h"


CExpenseItem::CExpenseItem(WCHAR * _lpzstrDescription, WCHAR * _lpzstrType, const int & _nCost, const Date & _AddDate)
{
	m_strDescription = wstring(_lpzstrDescription);
	m_strType = wstring(_lpzstrType);
	m_nCost = _nCost;
	m_AddDate = _AddDate;
}

CExpenseItem::~CExpenseItem()
{
}

WCHAR * CExpenseItem::GetDescription() const
{
	return _wcsdup(m_strDescription.c_str());
}

WCHAR * CExpenseItem::GetCategory() const
{
	return _wcsdup(m_strType.c_str());
}

int CExpenseItem::GetCost() const
{
	return m_nCost;
}

WCHAR * CExpenseItem::GetCostWStr() const
{
	return _wcsdup(to_wstring(m_nCost).c_str());
}

Date CExpenseItem::GetDate() const
{
	return  m_AddDate;
}
