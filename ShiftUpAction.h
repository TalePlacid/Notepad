#ifndef _SHIFTUPACTION_H //guard
#define _SHIFTUPACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftUpAction : public KeyAction {
public:
	ShiftUpAction(CWnd* parent);
	virtual ~ShiftUpAction();

	virtual void Perform();
};

#endif // !_SHIFTUPACTION_H
