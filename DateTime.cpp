#include "DateTime.h"
#include <ctime>
#include <cstdio>

#pragma warning(disable:4996)

DateTime::DateTime() {
	this->year = 1900;
	this->month = JAN;
	this->day = 1;
	this->weekday = MON;
	this->hour = 0;
	this->minute = 0;
	this->second = 0;
}

DateTime::DateTime(int year, Month month, int day, int hour, int minute, int second) {
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

DateTime::DateTime(char(*date)) {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	struct tm date_ = { 0, };

	sscanf(date, "%4d-%2d-%2d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);

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

DateTime::DateTime(const DateTime& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekday = source.weekday;
	this->hour = source.hour;
	this->minute = source.minute;
	this->second = source.second;
}

DateTime::~DateTime() {

}

DateTime& DateTime::operator=(const DateTime& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekday = source.weekday;
	this->hour = source.hour;
	this->minute = source.minute;
	this->second = source.second;

	return *this;
}

DateTime DateTime::Today() {
	time_t timer;
	struct tm* today_;
	DateTime today;

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

DateTime DateTime::Now() {
	time_t timer;
	struct tm* now_;
	DateTime now;

	time(&timer);
	now_ = localtime(&timer);

	now.year = now_->tm_year + 1900;
	now.month = static_cast<Month>(now_->tm_mon + 1);
	now.day = now_->tm_mday;
	now.weekday = static_cast<Weekday>(now_->tm_wday);
	now.hour = now_->tm_hour;
	now.minute = now_->tm_min;
	now.second = now_->tm_sec;

	return now;
}

DateTime DateTime::Yesterday() {
	struct tm yesterday_ = { 0, };
	DateTime yesterday;

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



DateTime DateTime::Tomorrow() {
	struct tm tomorrow_ = { 0, };
	DateTime tomorrow;

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

DateTime DateTime::PreviousDateTime(int days) {
	struct tm previousDateTime_ = { 0, };
	DateTime previousDateTime;

	previousDateTime_.tm_year = this->year - 1900;
	previousDateTime_.tm_mon = this->month - 1;
	previousDateTime_.tm_mday = this->day - days;
	previousDateTime_.tm_hour = this->hour;
	previousDateTime_.tm_min = this->minute;
	previousDateTime_.tm_sec = this->second;

	mktime(&previousDateTime_);

	previousDateTime.year = previousDateTime_.tm_year + 1900;
	previousDateTime.month = static_cast<Month>(previousDateTime_.tm_mon + 1);
	previousDateTime.day = previousDateTime_.tm_mday;
	previousDateTime.weekday = static_cast<Weekday>(previousDateTime_.tm_wday);
	previousDateTime.hour = previousDateTime_.tm_hour;
	previousDateTime.minute = previousDateTime_.tm_min;
	previousDateTime.second = previousDateTime_.tm_sec;

	return previousDateTime;
}

DateTime DateTime::NextDateTime(int days) {
	struct tm nextDateTime_ = { 0, };
	DateTime nextDateTime;

	nextDateTime_.tm_year = this->year - 1900;
	nextDateTime_.tm_mon = this->month - 1;
	nextDateTime_.tm_mday = this->day + days;
	nextDateTime_.tm_hour = this->hour;
	nextDateTime_.tm_min = this->minute;
	nextDateTime_.tm_sec = this->second;

	mktime(&nextDateTime_);

	nextDateTime.year = nextDateTime_.tm_year + 1900;
	nextDateTime.month = static_cast<Month>(nextDateTime_.tm_mon + 1);
	nextDateTime.day = nextDateTime_.tm_mday;
	nextDateTime.weekday = static_cast<Weekday>(nextDateTime_.tm_wday);
	nextDateTime.hour = nextDateTime_.tm_hour;
	nextDateTime.minute = nextDateTime_.tm_min;
	nextDateTime.second = nextDateTime_.tm_sec;

	return nextDateTime;
}

DateTime DateTime::AddSeconds(int seconds) {
	struct tm date = { 0, };
	DateTime date_;

	date.tm_year = this->year - 1900;
	date.tm_mon = this->month - 1;
	date.tm_mday = this->day;
	date.tm_hour = this->hour;
	date.tm_min = this->minute;
	date.tm_sec = this->second + seconds;

	mktime(&date);

	date_.year = date.tm_year + 1900;
	date_.month = static_cast<Month>(date.tm_mon + 1);
	date_.day = date.tm_mday;
	date_.weekday = static_cast<Weekday>(date.tm_wday);
	date_.hour = date.tm_hour;
	date_.minute = date.tm_min;
	date_.second = date.tm_sec;

	return date_;
}

bool DateTime::IsEqual(const DateTime& other) {
	bool ret = false;

	if ((this->year == other.year)&&(this->month == other.month)&&(this->day == other.day)
		&&(this->hour == other.hour)&&(this->minute == other.minute)&&(this->second == other.second))
	{
		ret = true;
	}
	
	return ret;
}

bool DateTime::IsNotEqual(const DateTime& other) {
	bool ret = false;

	if ((this->year != other.year)||(this->month != other.month)||(this->day != other.day)
		||(this->hour != other.hour)||(this->minute != other.minute)||(this->second != other.second))
	{
		ret = true;
	}

	return ret;
}

bool DateTime::IsLesserThan(const DateTime& other) {
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

bool DateTime::IsGreaterThan(const DateTime& other) {
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

DateTime& DateTime::operator--() {
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

DateTime DateTime::operator--(int) {
	struct tm yesterday_ = { 0, };
	DateTime yesterday(*this);

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

DateTime DateTime::operator-(int days) {
	struct tm previousDateTime_ = { 0, };
	DateTime previousDateTime;

	previousDateTime_.tm_year = this->year - 1900;
	previousDateTime_.tm_mon = this->month - 1;
	previousDateTime_.tm_mday = this->day - days;
	previousDateTime_.tm_hour = this->hour;
	previousDateTime_.tm_min = this->minute;
	previousDateTime_.tm_sec = this->second;

	mktime(&previousDateTime_);

	previousDateTime.year = previousDateTime_.tm_year + 1900;
	previousDateTime.month = static_cast<Month>(previousDateTime_.tm_mon + 1);
	previousDateTime.day = previousDateTime_.tm_mday;
	previousDateTime.weekday = static_cast<Weekday>(previousDateTime_.tm_wday);
	previousDateTime.hour = previousDateTime_.tm_hour;
	previousDateTime.minute = previousDateTime_.tm_min;
	previousDateTime.second = previousDateTime_.tm_sec;

	return previousDateTime;
}

DateTime& DateTime::operator++() {
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

DateTime DateTime::operator++(int) {
	struct tm tomorrow_ = { 0, };
	DateTime tomorrow(*this);

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

DateTime DateTime::operator+(int days) {
	struct tm nextDateTime_ = { 0, };
	DateTime nextDateTime;

	nextDateTime_.tm_year = this->year - 1900;
	nextDateTime_.tm_mon = this->month - 1;
	nextDateTime_.tm_mday = this->day + days;
	nextDateTime_.tm_hour = this->hour;
	nextDateTime_.tm_min = this->minute;
	nextDateTime_.tm_sec = this->second;

	mktime(&nextDateTime_);

	nextDateTime.year = nextDateTime_.tm_year + 1900;
	nextDateTime.month = static_cast<Month>(nextDateTime_.tm_mon + 1);
	nextDateTime.day = nextDateTime_.tm_mday;
	nextDateTime.weekday = static_cast<Weekday>(nextDateTime_.tm_wday);
	nextDateTime.hour = nextDateTime_.tm_hour;
	nextDateTime.minute = nextDateTime_.tm_min;
	nextDateTime.second = nextDateTime_.tm_sec;

	return nextDateTime;
}

bool DateTime::operator==(const DateTime& other) {
	bool ret = false;

	if ((this->year == other.year) && (this->month == other.month) && (this->day == other.day)
		&& (this->hour == other.hour) && (this->minute == other.minute) && (this->second == other.second))
	{
		ret = true;
	}

	return ret;
}

bool DateTime::operator!=(const DateTime& other) {
	bool ret = false;

	if ((this->year != other.year) || (this->month != other.month) || (this->day != other.day)
		|| (this->hour != other.hour) || (this->minute != other.minute) || (this->second != other.second))
	{
		ret = true;
	}

	return ret;
}

bool DateTime::operator<(const DateTime& other) {
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

bool DateTime::operator<=(const DateTime& other) {
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

bool DateTime::operator>(const DateTime& other) {
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

bool DateTime::operator>=(const DateTime& other) {
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
	
	DateTime today = DateTime::Today();
	cout << today.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << today.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << today.GetDay()
		<< endl;

	DateTime yesterday = today.Yesterday();
	cout << yesterday.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << yesterday.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << yesterday.GetDay()
		<< endl;

	DateTime tomorrow = today.Tomorrow();
	cout << tomorrow.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << tomorrow.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << tomorrow.GetDay()
		<< endl;

	DateTime previosDateTime = tomorrow.PreviousDateTime(2);
	cout << previosDateTime.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << previosDateTime.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << previosDateTime.GetDay()
		<< endl;

	DateTime nextDateTime = today.NextDateTime(2);
	cout << nextDateTime.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << nextDateTime.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << nextDateTime.GetDay()
		<< endl;

	bool ret = yesterday.IsEqual(previosDateTime);
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

	ret = nextDateTime.IsGreaterThan(today);
	if (ret == true)
	{
		cout << "참입니다."
			<< endl;
	}

	DateTime birthday(1993, JUN, 9);
	cout << birthday.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << birthday.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << birthday.GetDay()
		<< endl;

	DateTime twoDaysBefore = --birthday - 1;
	cout << twoDaysBefore.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysBefore.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysBefore.GetDay()
		<< endl;

	DateTime birthdayTomorrow = ++birthday + 1;
	cout << birthdayTomorrow.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << birthdayTomorrow.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << birthdayTomorrow.GetDay()
		<< endl;

	DateTime twoDaysAfter = birthday++ + 2;
	cout << twoDaysAfter.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysAfter.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << twoDaysAfter.GetDay()
		<< endl;

	DateTime birthDayYesterday = birthday-- - 2;
	cout << birthDayYesterday.GetYear()
		<< "-"
		<< setw(2) << setfill('0') << birthDayYesterday.GetMonth()
		<< "-"
		<< setw(2) << setfill('0') << birthDayYesterday.GetDay()
		<< endl;

	
	DateTime stringMade(const_cast<char(*)>("19930609"));
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
