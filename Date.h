#ifndef _DATE_H //guard
#define _DATE_H

enum Month { JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 };

enum Weekday { SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6 };

class Date {
public:
	Date();
	Date(int year, Month month, int day, int hour, int minute, int second);
	Date(char(*date));
	~Date();
	Date(const Date& source);
	Date& operator=(const Date& source);

	static Date Today();
	Date Yesterday();
	Date Tomorrow();
	Date PreviousDate(int days);
	Date NextDate(int days);

	bool IsEqual(const Date& other);
	bool IsNotEqual(const Date& other);
	bool IsLesserThan(const Date& other);
	bool IsGreaterThan(const Date& other);

	int GetYear() const;
	Month GetMonth() const;
	int GetDay() const;
	Weekday GetWeekday() const;
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;

	Date& operator--();
	Date operator--(int);
	Date& operator++();
	Date operator++(int);
	Date operator -(int days);
	Date operator +(int days);

	bool operator==(const Date& other);
	bool operator!=(const Date& other);
	bool operator<(const Date& other);
	bool operator<=(const Date& other);
	bool operator>(const Date& other);
	bool operator>=(const Date& other);

private:
	int year;
	Month month;
	int day;
	Weekday weekday;
	int hour;
	int minute;
	int second;
};

inline int Date::GetYear() const {
	return this->year;
}

inline Month Date::GetMonth() const {
	return this->month;
}

inline int Date::GetDay() const {
	return this->day;
}

inline Weekday Date::GetWeekday() const {
	return this->weekday;
}

inline int Date::GetHour() const {
	return this->hour;
}

inline int Date::GetMinute() const {
	return this->minute;
}

inline int Date::GetSecond() const {
	return this->second;
}

#endif // !_DATE_H
