#ifndef _FINDREPLACEOPTION_H
#define _FINDREPLACEOPTION_H
#include <afxwin.h>

struct FindReplaceOption {
	CString findString;
	CString replaceString;
	BOOL isMatchWhole;
	BOOL isMatchCase;
	BOOL isSearchDown;

	FindReplaceOption(CString findString = "", CString replaceString = "", BOOL isMatchWhole = FALSE, BOOL isMatchCase = FALSE, BOOL isSearchDown = FALSE);
	~FindReplaceOption();
	FindReplaceOption(const FindReplaceOption& source);	
	FindReplaceOption& operator=(const FindReplaceOption& source);

	BOOL EqualsExceptSearchDirection(FindReplaceOption& other);
	BOOL operator==(FindReplaceOption& other);
	BOOL operator!=(FindReplaceOption& other);
};

#endif // !_FINDREPLACEOPTION_H
