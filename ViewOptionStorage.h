#ifndef _VIEWOPTIONSTORAGE_H //guard
#define _VIEWOPTIONSTORAGE_H
#include <afxwin.h>

class ViewOptionStorage {
public:
	ViewOptionStorage();
	~ViewOptionStorage();

	static BOOL Load(LOGFONT& logFont, BOOL& isAutoWrapped, double& magnification, BOOL& isStatusBarVisible);
	static void Save(const LOGFONT& logFont, BOOL isAutoWrapped, double magnification, BOOL isStatusBarVisible);
};

#endif // !_VIEWOPTIONSTORAGE_H
