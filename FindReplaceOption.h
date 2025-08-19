#ifndef _FINDREPLACEOPTION_H
#define _FINDREPLACEOPTION_H
#include <afxwin.h>

struct FindReplaceOption {
	CString findString;
	BOOL isMatchWhole;
	BOOL isMatchCase;
	BOOL isSearchDown;

	FindReplaceOption(CString findString = "", BOOL isMatchWhole = FALSE, BOOL isMatchCase = FALSE, BOOL isSearchDown = FALSE);

	BOOL EqualsExceptSearchDirection(FindReplaceOption& other);
	BOOL operator==(FindReplaceOption& other);
	BOOL operator!=(FindReplaceOption& other);
};

#endif // !_FINDREPLACEOPTION_H
