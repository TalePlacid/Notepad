#include "FindReplaceOption.h"

#pragma warning(disable:4996)

FindReplaceOption::FindReplaceOption(CString findString, BOOL isMatchWhole, BOOL isMatchCase, BOOL isSearchDown) {
	this->findString = findString;
	this->isMatchWhole = isMatchWhole;
	this->isMatchCase = isMatchCase;
	this->isSearchDown = isSearchDown;
}

BOOL FindReplaceOption::EqualsExceptSearchDirection(FindReplaceOption& other) {
	return (this->findString == other.findString)
		&& (this->isMatchWhole == other.isMatchWhole)
		&& (this->isMatchCase == other.isMatchCase)
		&& (this->isSearchDown != other.isSearchDown);
}

BOOL FindReplaceOption::operator==(FindReplaceOption& other) {
	return (this->findString == other.findString)
		&& (this->isMatchWhole == other.isMatchWhole)
		&& (this->isMatchCase == other.isMatchCase)
		&& (this->isSearchDown == other.isSearchDown);
}

BOOL FindReplaceOption::operator!=(FindReplaceOption& other) {
	return (this->findString != other.findString)
		|| (this->isMatchWhole != other.isMatchWhole)
		|| (this->isMatchCase != other.isMatchCase)
		|| (this->isSearchDown != other.isSearchDown);
}
