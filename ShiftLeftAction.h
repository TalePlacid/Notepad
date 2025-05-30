#ifndef _SHIFTLEFTACTION_H //guard
#define _SHIFTLEFTACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftLeftAction : public KeyAction {
public:
	ShiftLeftAction(CWnd* parent);
	virtual ~ShiftLeftAction();

	virtual void Perform();
};

#endif // !_SHIFTLEFTACTION_H
