#ifndef _FINDREPLACEINTERPRETER_H //guard
#define _FINDREPLACEINTERPRETER_H
#include <afxwin.h>
#include "AppID.h"

class FindReplaceInterpreter {
public:
	FindReplaceInterpreter();
	~FindReplaceInterpreter();

	static AppID DetermineID(LPARAM lParam);
	static BOOL IsCommand(AppID appID);
};

#endif // !_FINDREPLACEINTERPRETER_H
