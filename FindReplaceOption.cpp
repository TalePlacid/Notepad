#include "FindReplaceOption.h"

#pragma warning(disable:4996)

FindReplaceOption::FindReplaceOption(CString findString, CString replaceString, BOOL isMatchWhole, BOOL isMatchCase, BOOL isSearchDown)
	:findString(findString), replaceString(replaceString){
	this->isMatchWhole = isMatchWhole;
	this->isMatchCase = isMatchCase;
	this->isSearchDown = isSearchDown;
}

FindReplaceOption::~FindReplaceOption() {

}

FindReplaceOption::FindReplaceOption(const FindReplaceOption& source)
	:findString(source.findString), replaceString(source.replaceString) {
	this->isMatchWhole = source.isMatchWhole;
	this->isMatchCase = source.isMatchCase;
	this->isSearchDown = source.isSearchDown;
}

FindReplaceOption& FindReplaceOption::operator=(const FindReplaceOption& source) {
	this->findString = source.findString;
	this->replaceString = source.replaceString;
	this->isMatchWhole = source.isMatchWhole;
	this->isMatchCase = source.isMatchCase;
	this->isSearchDown = source.isSearchDown;

	return *this;
}

BOOL FindReplaceOption::IsEqual(const FindReplaceOption& other) {
	return (this->findString == other.findString)
		&& (this->replaceString == other.replaceString)
		&& (this->isMatchWhole == other.isMatchWhole)
		&& (this->isMatchCase == other.isMatchCase)
		&& (this->isSearchDown == other.isSearchDown);
}

BOOL FindReplaceOption::EqualsExceptSearchDirection(const FindReplaceOption& other) {
	return (this->findString == other.findString)
		&& (this->replaceString == other.replaceString)
		&& (this->isMatchWhole == other.isMatchWhole)
		&& (this->isMatchCase == other.isMatchCase);
}

BOOL FindReplaceOption::operator==(const FindReplaceOption& other) {
	return (this->findString == other.findString)
		&& (this->replaceString == other.replaceString)
		&& (this->isMatchWhole == other.isMatchWhole)
		&& (this->isMatchCase == other.isMatchCase)
		&& (this->isSearchDown == other.isSearchDown);
}

BOOL FindReplaceOption::operator!=(const FindReplaceOption& other) {
	return (this->findString != other.findString)
		|| (this->replaceString != other.replaceString)
		|| (this->isMatchWhole != other.isMatchWhole)
		|| (this->isMatchCase != other.isMatchCase)
		|| (this->isSearchDown != other.isSearchDown);
}

