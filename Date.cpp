#include "Date.h"
#include <ctime>
#include <cstdio>

#pragma warning(disable:4996)

Date::Date() {
	this->year = 1900;
	this->month = JAN;
	this->day = 1;
	this->weekday = MON;
	this->hour = 0;
	this->minute = 0;
	this->second = 0;
}

Date::Date(int year, Month month, int day, int hour, int minute, int second) {
	struct tm date = { 0, };

	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;
	date.tm_hour = hour;
	date.tm_min = minute;
	date.tm_sec = second;

	mktime(&date);

	this->year = date.tm_year + 1900;
	this->month = static_cast<Month>(date.tm_mon + 1);
	this->day = date.tm_mday;
	this->weekday = static_cast<Weekday>(date.tm_wday);
	this->hour = date.tm_hour;
	this->minute = date.tm_min;
	this->second = date.tm_sec;
}

Date::Date(char(*date)) {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	struct tm date_ = { 0, };

	sscanf(date, "%4d%2d%2d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);

	date_.tm_year = year - 1900;
	date_.tm_mon = month - 1;
	date_.tm_mday = day;
	date_.tm_hour = hour;
	date_.tm_min = minute;

	mktime(&date_);

	this->year = date_.tm_year + 1900;
	this->month = static_cast<Month>(date_.tm_mon + 1);
	this->day = date_.tm_mday;
	this->weekday = static_cast<Weekday>(date_.tm_wday);
	this->hour = date_.tm_hour;
	this->minute = date_.tm_min;
	this->second = date_.tm_sec;
}

Date::Date(const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekday = source.weekday;
	this->hour = source.hour;
	this->minute = source.minute;
	this->second = source.second;
}

Date::~Date() {

}

Date& Date::operator=(const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekday = source.weekday;
	this->hour = source.hour;
	this->minute = source.minute;
	this->second = source.second;

	return *this;
}

Date Date::Today() {
	time_t timer;
	struct tm* today_;
	Date today;

	time(&timer);
	today_ = localtime(&timer);

	today.year = today_->tm_year + 1900;
	today.month = static_cast<Month>(today_->tm_mon + 1);
	today.day = today_->tm_mday;
	today.weekday = static_cast<Weekday>(today_->tm_wday);
	today.hour = today_->tm_hour;
	today.minute = today_->tm_min;
	today.second = today_->tm_sec;

	return today;
}

Date Date::Yesterday() {
	struct tm yesterday_ = { 0, };
	Date yesterday;

	yesterday_.tm_year = this->year - 1900;
	yesterday_.tm_mon = this->month - 1;
	yesterday_.tm_mday = this->day - 1;
	yesterday_.tm_hour = this->hour;
	yesterday_.tm_min = this->minute;
	yesterday_.tm_sec = this->second;

	mktime(&yesterday_);

	yesterday.year = yesterday_.tm_year + 1900;
	yesterday.month = static_cast<Month>(yesterday_.tm_mon + 1);
	yesterday.day = yesterday_.tm_mday;
	yesterday.weekday = static_cast<Weekday>(yesterday_.tm_wday);
	yesterday.hour = yesterday_.tm_hour;
	yesterday.minute = yesterday_.tm_min;
	yesterday.second = yesterday_.tm_sec;

	return yesterday;
}

Date Date::Tomorrow() {
	struct tm tomorrow_ = { 0, };
	Date tomorrow;

	tomorrow_.tm_year = this->year - 1900;
	tomorrow_.tm_mon = this->month - 1;
	tomorrow_.tm_mday = this->day + 1;
	tomorrow_.tm_hour = this->hour;
	tomorrow_.tm_min = this->minute;
	tomorrow_.tm_sec = this->second;

	mktime(&tomorrow_);

	tomorrow.year = tomorrow_.tm_year + 1900;
	tomorrow.month = static_cast<Month>(tomorrow_.tm_mon + 1);
	tomorrow.day = tomorrow_.tm_mday;
	tomorrow.weekday = static_cast<Weekday>(tomorrow_.tm_wday);
	tomorrow.hour = tomorrow_.tm_hour;
	tomorrow.minute = tomorrow_.tm_min;
	tomorrow.second = tomorrow_.tm_sec;

	return tomorrow;
}

Date Date::PreviousDate(int days) {
	struct tm previousDate_ = { 0, };
	Date previousDate;

	previousDate_.tm_year = this->year - 1900;
	previousDate_.tm_mon = this->month - 1;
	previousDate_.tm_mday = this->day - days;
	previousDate_.tm_hour = this->hour;
	previousDate_.tm_min = this->minute;
	previousDate_.tm_sec = this->second;

	mktime(&previousDate_);

	previousDate.year = previousDate_.tm_year + 1900;
	previousDate.month = static_cast<Month>(previousDate_.tm_mon + 1);
	previousDate.day = previousDate_.tm_mday;
	previousDate.weekday = static_cast<Weekday>(previousDate_.tm_wday);
	previousDate.hour = previousDate_.tm_hour;
	previousDate.minute = previousDate_.tm_min;
	previousDate.second = previousDate_.tm_sec;

	return previousDate;
}

Date Date::NextDate(int days) {
	struct tm nextDate_ = { 0, };
	Date nextDate;

	nextDate_.tm_year = this->year - 1900;
	nextDate_.tm_mon = this->month - 1;
	nextDate_.tm_mday = this->day + days;
	nextDate_.tm_hour = this->hour;
	nextDate_.tm_min = this->minute;
	nextDate_.tm_sec = this->second;

	mktime(&nextDate_);

	nextDate.year = nextDate_.tm_year + 1900;
	nextDate.month = static_cast<Month>(nextDate_.tm_mon + 1);
	nextDate.day = nextDate_.tm_mday;
	nextDate.weekday = static_cast<Weekday>(nextDate_.tm_wday);
	nextDate.hour = nextDate_.tm_hour;
	nextDate.minute = nextDate_.tm_min;
	nextDate.second = nextDate_.tm_sec;

	return nextDate;
}

bool Date::IsEqual(const Date& other) {
	bool ret = false;

	if ((this->year == other.year)&&(this->month == other.month)&&(this->day == other.day)
		&&(this->hour == other.hour)&&(this->minute == other.minute)&&(this->second == other.second))
	{
		ret = true;
	}
	
	return ret;
}

bool Date::IsNotEqual(const Date& other) {
	bool ret = false;

	if ((this->year != other.year)||(this->month != other.month)||(this->day != other.day)
		||(this->hour != other.hour)||(this->minute != other.minute)||(this->second != other.second))
	{
		ret = true;
	}

	return ret;
}

bool Date::IsLesserThan(const Date& other) {
	bool ret = false;

	if (this->year < other.year)
	{
		ret = true;
	}
	else if ((this->year == other.year)&&(this->month < other.month))
	{
		ret = true;
	}
	else if ((this->year == other.year)&&(this->month == other.month)&&(this->day < other.day))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour < other.hour))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute < other.minute))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second < other.second))
	{
		ret = true;
	}

	return ret;
}

bool Date::IsGreaterThan(const Date& other) {
	bool ret = false;

	if (this->year > other.year)
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month > other.month))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day > other.day))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour > other.hour))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute > other.minute))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second > other.second))
	{
		ret = true;
	}

	return ret;
}

Date& Date::operator--() {
	struct tm yesterday = { 0, };

	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1;
	yesterday.tm_hour = this->hour;
	yesterday.tm_min = this->minute;
	yesterday.tm_sec = this->second;

	mktime(&yesterday);

	this->year = yesterday.tm_year + 1900;
	this->month = static_cast<Month>(yesterday.tm_mon + 1);
	this->day = yesterday.tm_mday;
	this->weekday = static_cast<Weekday>(yesterday.tm_wday);
	this->hour = yesterday.tm_hour;
	this->minute = yesterday.tm_min;
	this->second = yesterday.tm_sec;

	return *this;
}

Date Date::operator--(int) {
	struct tm yesterday_ = { 0, };
	Date yesterday(*this);

	yesterday_.tm_year = this->year - 1900;
	yesterday_.tm_mon = this->month - 1;
	yesterday_.tm_mday = this->day - 1;
	yesterday_.tm_hour = this->hour;
	yesterday_.tm_min = this->minute;
	yesterday_.tm_sec = this->second;

	mktime(&yesterday_);

	this->year = yesterday_.tm_year + 1900;
	this->month = static_cast<Month>(yesterday_.tm_mon + 1);
	this->day = yesterday_.tm_mday;
	this->weekday = static_cast<Weekday>(yesterday_.tm_wday);
	this->hour = yesterday_.tm_hour;
	this->minute = yesterday_.tm_min;
	this->second = yesterday_.tm_sec;

	return yesterday;
}

Date Date::operator-(int days) {
	struct tm previousDate_ = { 0, };
	Date previousDate;

	previousDate_.tm_year = this->year - 1900;
	previousDate_.tm_mon = this->month - 1;
	previousDate_.tm_mday = this->day - days;
	previousDate_.tm_hour = this->hour;
	previousDate_.tm_min = this->minute;
	previousDate_.tm_sec = this->second;

	mktime(&previousDate_);

	previousDate.year = previousDate_.tm_year + 1900;
	previousDate.month = static_cast<Month>(previousDate_.tm_mon + 1);
	previousDate.day = previousDate_.tm_mday;
	previousDate.weekday = static_cast<Weekday>(previousDate_.tm_wday);
	previousDate.hour = previousDate_.tm_hour;
	previousDate.minute = previousDate_.tm_min;
	previousDate.second = previousDate_.tm_sec;

	return previousDate;
}

Date& Date::operator++() {
	struct tm tomorrow = { 0, };

	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1;
	tomorrow.tm_hour = this->hour;
	tomorrow.tm_min = this->minute;
	tomorrow.tm_sec = this->second;

	mktime(&tomorrow);

	this->year = tomorrow.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow.tm_mon + 1);
	this->day = tomorrow.tm_mday;
	this->weekday = static_cast<Weekday>(tomorrow.tm_wday);
	this->hour = tomorrow.tm_hour;
	this->minute = tomorrow.tm_min;
	this->second = tomorrow.tm_sec;

	return *this;
}

Date Date::operator++(int) {
	struct tm tomorrow_ = { 0, };
	Date tomorrow(*this);

	tomorrow_.tm_year = this->year - 1900;
	tomorrow_.tm_mon = this->month - 1;
	tomorrow_.tm_mday = this->day + 1;
	tomorrow_.tm_hour = this->hour;
	tomorrow_.tm_min = this->minute;
	tomorrow_.tm_sec = this->second;

	mktime(&tomorrow_);

	this->year = tomorrow_.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow_.tm_mon + 1);
	this->day = tomorrow_.tm_mday;
	this->weekday = static_cast<Weekday>(tomorrow_.tm_wday);
	this->hour = tomorrow_.tm_hour;
	this->minute = tomorrow_.tm_min;
	this->second = tomorrow_.tm_sec;

	return tomorrow;
}

Date Date::operator+(int days) {
	struct tm nextDate_ = { 0, };
	Date nextDate;

	nextDate_.tm_year = this->year - 1900;
	nextDate_.tm_mon = this->month - 1;
	nextDate_.tm_mday = this->day + days;
	nextDate_.tm_hour = this->hour;
	nextDate_.tm_min = this->minute;
	nextDate_.tm_sec = this->second;

	mktime(&nextDate_);

	nextDate.year = nextDate_.tm_year + 1900;
	nextDate.month = static_cast<Month>(nextDate_.tm_mon + 1);
	nextDate.day = nextDate_.tm_mday;
	nextDate.weekday = static_cast<Weekday>(nextDate_.tm_wday);
	nextDate.hour = nextDate_.tm_hour;
	nextDate.minute = nextDate_.tm_min;
	nextDate.second = nextDate_.tm_sec;

	return nextDate;
}

bool Date::operator==(const Date& other) {
	bool ret = false;

	if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second == other.second))
	{
		ret = true;
	}

	return ret;
}

bool Date::operator!=(const Date& other) {
	bool ret = false;

	if ((this->year != other.year) || (this->month != other.month) || (this->day != other.day)
		|| (this->hour != other.hour) || (this->minute != other.minute) || (this->second != other.second))
	{
		ret = true;
	}

	return ret;
}

bool Date::operator<(const Date& other) {
	bool ret = false;

	if (this->year < other.year)
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month < other.month))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day < other.day))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour < other.hour))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute < other.minute))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second < other.second))
	{
		ret = true;
	}

	return ret;
}

bool Date::operator<=(const Date& other) {
	bool ret = false;

	if (this->year < other.year)
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month < other.month))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day < other.day))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour < other.hour))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute < other.minute))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second <= other.second))
	{
		ret = true;
	}
	return ret;
}

bool Date::operator>(const Date& other) {
	bool ret = false;

	if (this->year > other.year)
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month > other.month))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day > other.day))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour > other.hour))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute > other.minute))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second > other.second))
	{
		ret = true;
	}

	return ret;
}

bool Date::operator>=(const Date& other) {
	bool ret = false;

	if (this->year > other.year)
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month > other.month))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day > other.day))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour > other.hour))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute > other.minute))
	{
		ret = true;
	}
	else if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second >= other.second))
	{
		ret = true;
	}

	return ret;
}

#if 0

#include <iostream>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[]) {
	
	Date today = Date::Today();
	cout << today.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << today.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << today.GetDay()
		<< endl;

	Date yesterday = today.Yesterday();
	cout << yesterday.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << yesterday.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << yesterday.GetDay()
		<< endl;

	Date tomorrow = today.Tomorrow();
	cout << tomorrow.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << tomorrow.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << tomorrow.GetDay()
		<< endl;

	Date previosDate = tomorrow.PreviousDate(2);
	cout << previosDate.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << previosDate.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << previosDate.GetDay()
		<< endl;

	Date nextDate = today.NextDate(2);
	cout << nextDate.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << nextDate.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << nextDate.GetDay()
		<< endl;

	bool ret = yesterday.IsEqual(previosDate);
	if (ret == true)
	{
		cout << "참입니다."
			<< endl;
	}

	ret = yesterday.IsNotEqual(today);
	if (ret == true)
	{
		cout << "참입니다."
			<< endl;
	}

	ret = yesterday.IsLesserThan(today);
	if (ret == true)
	{
		cout << "참입니다."
			<< endl;
	}

	ret = nextDate.IsGreaterThan(today);
	if (ret == true)
	{
		cout << "참입니다."
			<< endl;
	}

	Date birthday(1993, JUN, 9);
	cout << birthday.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << birthday.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << birthday.GetDay()
		<< endl;

	Date twoDaysBefore = --birthday - 1;
	cout << twoDaysBefore.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysBefore.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysBefore.GetDay()
		<< endl;

	Date birthdayTomorrow = ++birthday + 1;
	cout << birthdayTomorrow.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << birthdayTomorrow.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << birthdayTomorrow.GetDay()
		<< endl;

	Date twoDaysAfter = birthday++ + 2;
	cout << twoDaysAfter.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysAfter.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysAfter.GetDay()
		<< endl;

	Date birthDayYesterday = birthday-- - 2;
	cout << birthDayYesterday.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << birthDayYesterday.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << birthDayYesterday.GetDay()
		<< endl;

	
	Date stringMade(const_cast<char(*)>("19930609"));
	cout << stringMade.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << stringMade.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << stringMade.GetDay()
		<< endl;

	if (birthday == stringMade)
	{
		cout << "참입니다."
			<< endl;
	}

	if (birthday != birthDayYesterday)
	{
		cout << "참입니다."
			<< endl;
	}

	if (birthday < twoDaysAfter)
	{
		cout << "참입니다."
			<< endl;
	}

	if (birthday <= stringMade)
	{
		cout << "참입니다."
			<< endl;
	}

	if (birthday > twoDaysBefore)
	{
		cout << "참입니다."
			<< endl;
	}

	if (birthday >= stringMade)
	{
		cout << "참입니다."
			<< endl;
	}

	return 0;
}

#endif