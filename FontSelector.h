#ifndef _FONTSELECTOR_H //guard
#define _FONTSELECTOR_H

#include <afxwin.h>

class FontSelector {
public:
	FontSelector();
	~FontSelector();

	LOGFONT SelectBaseLogFont(CWnd* owner);
	LOGFONT SelectScaledLogFont(LOGFONT baseLogFont, double magnification);
};

#endif // !_FONTSELECTOR_H
