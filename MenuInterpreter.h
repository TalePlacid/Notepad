#ifndef _MENUINTERPRETER_H //guard
#define _MENUINTERPRETER_H
#include <afxwin.h>
#include "AppID.h"

class MenuInterpreter {
public:
	MenuInterpreter();
	~MenuInterpreter();

	static BOOL IsAction(UINT nID);
	static AppID DetermineID(UINT nID);
};

#endif // !_MENUINTERPRETER_H
