#include "stdafx.h"
#include "Date.h"
#include <math.h>
#include <iomanip>
#include <ctime>

using namespace std;
Date::Date()
{
	time_t t = time(0);   // get time now
	struct tm * now = new struct tm;
	localtime_s(now, &t);
	m_iYear = (now->tm_year + 1900);
	m_iMonth = (now->tm_mon + 1);
	m_iDay = now->tm_mday;
	delete now;
}

Date::Date(const Date & date)
{
	*this = date;
}

Date::Date(const int & iDay, const int & iMonth, const int & iYear)
{
	m_iYear = iYear;
	m_iMonth = iMonth;
	m_iDay = iDay;
}
Date::Date(const unsigned int & orderOfDay)
{
	unsigned int temp = orderOfDay;
	time_t t = time(0);   // get time now
	struct tm * now = new struct tm;
	localtime_s(now, &t);
	m_iYear = (now->tm_year + 1900);
	m_iMonth = (now->tm_mon + 1);
	m_iDay = now->tm_mday;

	while (temp > (unsigned int)365 + IsLeapYear(m_iYear))
	{
		++m_iYear;
		temp -= 365 + IsLeapYear(m_iYear);
	}
	while (temp > (unsigned int)DayInMonthYear(m_iMonth, m_iYear))
	{
		++m_iMonth;
		temp -= DayInMonthYear(m_iMonth, m_iYear);
	}
	m_iDay = int(temp);
}

Date::Date(const unsigned int & orderOfDay, const int & year)
{
	unsigned int temp = orderOfDay;

	m_iYear = year;
	m_iMonth = 1;

	while (temp > (unsigned int)365 + IsLeapYear(m_iYear))
	{
		++m_iYear;
		temp -= 365 + IsLeapYear(m_iYear);
	}
	while (temp > (unsigned int)DayInMonthYear(m_iMonth, m_iYear))
	{
		++m_iMonth;
		temp -= DayInMonthYear(m_iMonth, m_iYear);
	}
	m_iDay = int(temp);
}

Date::~Date()
{
}


bool Date::IsLeapYear(int const &iYear)
{
	if (iYear % 4 == 0 && iYear % 100 != 0 || 400 == 0)
		return true;
	return false;
}

int Date::DayInMonthYear(int const &iMonth, int const &iYear)
{
	switch (iMonth)
	{
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
	{
		return 31;
	};
	case 4: case 6: case 9: case 11:
	{
		return 30;
	}
	case 2:
	{
		return 28 + IsLeapYear(iYear);
	}
	default:
		return 0;
	}
}

int Date::DateCheck(const int & iDay, const int & iMonth, const int & iYear)
{
	switch (iMonth)
	{
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
	{
		if (iDay >= 1 && iDay <= 31)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	case 4: case 6: case 9: case 11:
	{
		if (iDay >= 1 && iDay <= 30)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	case 2:
	{
		if (iDay >= 1 && iDay <= 28 + abs(IsLeapYear(iYear)) % 2)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	default:
		return 0;
	}

}

int Date::GetDay() const
{
	return m_iDay;
}
int Date::GetMonth() const
{
	return m_iMonth;
}
int Date::GetYear() const
{
	return m_iYear;
}
int Date::GetDayOrder() const
{
	int iOrder = 0;
	for (int i = 1; i < m_iMonth; i++)
	{
		iOrder += DayInMonthYear(i, m_iYear);
	}
	iOrder += m_iDay;
	return iOrder;
}
short Date::GetDayOfWeek() const
{
	return (m_iYear - 2 + ((m_iYear - 1) / 4) - ((m_iYear - 1) / 100) + ((m_iYear - 1) / 400) + GetDayOrder()) % 7;
}

short Date::GetWeekOrder() const
{
	return GetDayOrder() / 7 + (Date(1, 1, 1).GetDayOfWeek() + GetDayOrder() % 7) >= 7 ? 1 : 0;
}

string Date::ToString(bool order) const
{
	if (order)
		return to_string(m_iDay) + "/" + to_string(m_iMonth) + "/" + to_string(m_iYear);
	return to_string(m_iYear) + "/" + to_string(m_iMonth) + "/" + to_string(m_iDay);
}

wstring Date::ToWString(bool order) const
{
	if (order)
		return to_wstring(m_iDay) + L"/" + to_wstring(m_iMonth) + L"/" + to_wstring(m_iYear);
	return to_wstring(m_iYear) + L"/" + to_wstring(m_iMonth) + L"/" + to_wstring(m_iDay);
}

bool Date::SetDay(const int & iDay)
{
	if (DateCheck(iDay, m_iMonth, m_iYear) == 0)
		return false;
	m_iDay = iDay;
	return true;
}
bool Date::SetMonth(const int & iMonth)
{
	if (DateCheck(m_iDay, iMonth, m_iYear) == 0)
		return false;
	m_iMonth = iMonth;
	return true;
}
bool Date::SetYear(const int & iYear)
{
	if (DateCheck(m_iDay, m_iMonth, iYear) == 0)
		return false;
	m_iYear = iYear;
	return true;
}
bool Date::SetDate(const Date & date)
{
	*this = date;
	return true;
}
bool Date::SetDate(const unsigned int & orderOfDay)
{
	unsigned int temp = orderOfDay;
	while (temp > (unsigned int)365 + IsLeapYear(m_iYear))
	{
		++m_iYear;
		temp -= 365 + IsLeapYear(m_iYear);
	}
	while (temp > (unsigned int)DayInMonthYear(m_iMonth, m_iYear))
	{
		++m_iMonth;
		temp -= DayInMonthYear(m_iMonth, m_iYear);
	}
	m_iDay = int(temp);
	return true;
}
bool Date::SetDate(const int & iDay, const int & iMonth, const int & iYear)
{
	if (DateCheck(iDay, iMonth, iYear) == 0)
		return false;
	m_iDay = iDay;
	m_iMonth = iMonth;
	m_iYear = iYear;
	return true;
}



Date Date::GenerateNextDate() const
{
	Date nextDate;
	if (m_iDay == DayInMonthYear(m_iMonth, m_iYear))
	{
		nextDate.m_iDay = 1;
		if (m_iMonth == 12)
		{
			nextDate.m_iMonth = 1;
			nextDate.m_iYear = m_iYear + 1;
		}
		else
		{
			nextDate.m_iMonth = m_iMonth + 1;
			nextDate.m_iYear = m_iYear;
		}
	}
	else
	{
		nextDate.m_iDay = m_iDay + 1;
		nextDate.m_iMonth = m_iMonth;
		nextDate.m_iYear = m_iYear;
	}
	return nextDate;
}

Date Date::GeneratePrevDate() const
{
	Date previusDate;
	if (m_iDay == 1)
	{
		if (m_iMonth == 1)
		{
			previusDate.m_iMonth = 12;
			previusDate.m_iYear = m_iYear - 1;
		}
		else
		{
			previusDate.m_iMonth = m_iMonth - 1;
			previusDate.m_iYear = m_iYear;
		}
		previusDate.m_iDay = DayInMonthYear(previusDate.m_iMonth, previusDate.m_iYear);
	}
	else
	{
		previusDate.m_iDay = m_iDay - 1;
		previusDate.m_iMonth = m_iMonth;
		previusDate.m_iYear = m_iYear;
	}
	return previusDate;
}

Date Date::GenerateNextDate(const int & iTimes) const
{
	if (iTimes == 0)
		return *this;
	if (iTimes < 0)
		return GeneratePrevDate(-iTimes);
	Date date = *this;
	for (int i = 0; i < iTimes; ++i)
		date = date.GenerateNextDate();
	return date;
}

Date Date::GeneratePrevDate(const int & iTimes) const
{
	if (iTimes == 0)
		return *this;
	if (iTimes < 0)
		return GenerateNextDate(-iTimes);
	Date date = *this;
	for (int i = 0; i < iTimes; ++i)
		date = date.GeneratePrevDate();
	return date;
}

int Date::operator ==(const Date & date) const
{
	return this->GetDayOrder() - date.GetDayOrder();
}
int Date::operator !=(const Date & date) const
{
	return !(*this == date);
}
int Date::operator <(const Date & date) const
{
	if ((*this == date) < 0)
		return 1;
	return 0;
}
int Date::operator <=(const Date & date) const
{
	if ((*this == date) <= 0)
		return 1;
	return 0;
}
int Date::operator >(const Date & date) const
{
	if ((*this == date) > 0)
		return 1;
	return 0;
}
int Date::operator >=(const Date & date) const
{
	if ((*this == date) >= 0)
		return 1;
	return 0;
}
int Date::operator -(const Date & date) const
{
	int k = 0;
	if (m_iYear < date.m_iYear)
	{
		k = Date(31, 12, this->m_iYear).GetDayOrder() - this->GetDayOrder() + 1;
		for (int i = m_iYear + 1; i < date.m_iYear; ++i)
			k += 365 + IsLeapYear(i);
		k *= -1;
	}
	else if (m_iYear > date.m_iYear)
	{
		k = Date(31, 12, date.m_iYear).GetDayOrder() - date.GetDayOrder() + 1;
		for (int i = this->m_iYear + 1; i < this->m_iYear; ++i)
			k += 365 + IsLeapYear(i);
	}
	k += GetDayOrder() - date.GetDayOrder();
	return k;
}
Date Date::operator +(const unsigned int & diff)
{
	Date date(diff, this->m_iYear);
	return date;
}

istream & operator >>(istream & in, Date & date)
{
	in >> date.m_iDay;
	in.ignore(1, 13);
	in >> date.m_iMonth;
	in.ignore(1, 13);
	in >> date.m_iYear;
	in.ignore(1, 13);
	return in;
}
ostream & operator <<(ostream & out, const Date & date)
{
	out << setfill('0') << setw(2) << date.m_iDay << '/' << date.m_iMonth << '/' << setfill('0') << setw(4) << date.m_iYear;
	return out;
}