#ifndef _PAGESETTING_H //guard
#define _PAGESETTING_H
#include <afxwin.h>

typedef signed long int Long;

struct Margin {
	Long left;
	Long right;
	Long up;
	Long down;
};

struct PageSetting {
	PageSetting();	
	PageSetting(CString paperName, BOOL isVertical, Margin margin, CString header, CString footer);
	~PageSetting();

	CString paperName;
	BOOL isVertical;
	Margin margin;
	CString header;
	CString footer;
};

#endif // !_PAGESETTING_H
