#ifndef _DATETIME_H //guard
#define _DATETIME_H

enum Month { JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 };

enum Weekday { SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6 };

class DateTime {
public:
	DateTime();
	DateTime(int year, Month month, int day, int hour, int minute, int second);
	DateTime(char(*date));
	~DateTime();
	DateTime(const DateTime& source);
	DateTime& operator=(const DateTime& source);

	static DateTime Today();
	static DateTime Now();
	DateTime Yesterday();
	DateTime Tomorrow();
	DateTime PreviousDateTime(int days);
	DateTime NextDateTime(int days);
	DateTime AddSeconds(int seconds);

	bool IsEqual(const DateTime& other);
	bool IsNotEqual(const DateTime& other);
	bool IsLesserThan(const DateTime& other);
	bool IsGreaterThan(const DateTime& other);

	int GetYear() const;
	Month GetMonth() const;
	int GetDay() const;
	Weekday GetWeekday() const;
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;

	DateTime& operator--();
	DateTime operator--(int);
	DateTime& operator++();
	DateTime operator++(int);
	DateTime operator -(int days);
	DateTime operator +(int days);

	bool operator==(const DateTime& other);
	bool operator!=(const DateTime& other);
	bool operator<(const DateTime& other);
	bool operator<=(const DateTime& other);
	bool operator>(const DateTime& other);
	bool operator>=(const DateTime& other);

private:
	int year;
	Month month;
	int day;
	Weekday weekday;
	int hour;
	int minute;
	int second;
};

inline int DateTime::GetYear() const {
	return this->year;
}

inline Month DateTime::GetMonth() const {
	return this->month;
}

inline int DateTime::GetDay() const {
	return this->day;
}

inline Weekday DateTime::GetWeekday() const {
	return this->weekday;
}

inline int DateTime::GetHour() const {
	return this->hour;
}

inline int DateTime::GetMinute() const {
	return this->minute;
}

inline int DateTime::GetSecond() const {
	return this->second;
}

#endif // !_DATETIME_H

