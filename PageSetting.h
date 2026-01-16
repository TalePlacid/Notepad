#ifndef _PAGESETTING_H //guard
#define _PAGESETTING_H
#include <afxwin.h>
#include "Paper.h"

struct PageSetting {
public:
	PageSetting();
	PageSetting(Long width, Long heght, RECT margin, CString header, CString footer);
	PageSetting(const PaperSize paperSize, RECT margin, CString header, CString footer);
	~PageSetting();
private:
	PaperSize paperSize;
	RECT margin;
	CString header;
	CString footer;
};

#endif // !_PAGESETTING_H
