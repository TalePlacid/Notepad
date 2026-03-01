#ifndef _MENUINTERPRETER_H //guard
#define _MENUINTERPRETER_H
#include <afxwin.h>
#include "AppID.h"

class MenuInterpreter {
public:
	MenuInterpreter();
	~MenuInterpreter();

	static BOOL IsFindReplace(UINT nID);
	static BOOL IsCommand(UINT nID);
	static AppID DetermineID(UINT nID);
};

#endif // !_MENUINTERPRETER_H

