#ifndef _KEYDOWNINTERPRETER_H //guard
#define _KEYDOWNINTERPRETER_H
#include <afxwin.h>
#include "AppID.h"

class KeyDownInterpreter {
public:
	KeyDownInterpreter();
	~KeyDownInterpreter();

	static BOOL IsAction(UINT nChar);
	static AppID DetermineID(UINT nChar);
};

#endif // !_KEYDOWNINTERPRETER_H
