#ifndef _FINDREPLACEINTERPRETER_H //guard
#define _FINDREPLACEINTERPRETER_H
#include <afxwin.h>
#include "AppID.h"

class CFindReplaceDialog;

class FindReplaceInterpreter {
public:
	FindReplaceInterpreter();
	~FindReplaceInterpreter();

	static AppID DetermineID(CFindReplaceDialog* findReplaceDialog);
};

#endif // !_FINDREPLACEINTERPRETER_H

