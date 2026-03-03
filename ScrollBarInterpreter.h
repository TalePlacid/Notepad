#ifndef _SCROLLBARINTERPRETER_H //guard
#define _SCROLLBARINTERPRETER_H
#include <afxwin.h>
#include "AppID.h"

class ScrollBarInterpreter {
public:
	ScrollBarInterpreter();
	~ScrollBarInterpreter();

	static AppID DetermineID(int nBar, UINT nSBCode);
};

#endif // !_SCROLLBARINTERPRETER_H
