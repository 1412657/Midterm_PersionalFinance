#pragma once

#include <iostream>
#include <string>

using namespace std;
class Date
{
private:
	int m_iDay;
	int m_iMonth;
	int m_iYear;

	//static enum DayOfWeek{Monday, TuesDay, Wednesday, Thurday, Saturday, Sunday};

public:
	Date();//default constructor set to current date
	Date(const Date &);
	Date(const int &, const int &, const int &);
	Date(const unsigned int &);
	Date(const unsigned int &, const int &);
	~Date();

	static bool IsLeapYear(const int & iYear);
	static int DayInMonthYear(const int & iMonth, const int & iYear);
	static int DateCheck(const int & iDay, const int & iMonth, const int & iYear);

	int GetDay() const;
	int GetMonth() const;
	int GetYear() const;
	int GetDayOrder() const;
	short GetDayOfWeek() const;
	short GetWeekOrder() const;//first day of week is monday
	string ToString(bool = true) const;
	wstring ToWString(bool = true) const;

	bool SetDay(const int &);
	bool SetMonth(const int &);
	bool SetYear(const int &);
	bool SetDate(const Date &);
	bool SetDate(const unsigned int &);
	bool SetDate(const int & iDay, const int & iMonth, const int & iYear);

	Date GenerateNextDate() const;
	Date GeneratePrevDate() const;
	Date GenerateNextDate(const int &) const;
	Date GeneratePrevDate(const int &) const;

	int operator ==(const Date &) const;
	int operator !=(const Date &) const;
	int operator <(const Date &) const;
	int operator <=(const Date &) const;
	int operator >(const Date &) const;
	int operator >=(const Date &) const;
	int operator -(const Date &) const;

	Date operator +(const unsigned int &);

	friend istream & operator >>(istream &, Date &);
	friend ostream & operator <<(ostream &, const Date &);
};

