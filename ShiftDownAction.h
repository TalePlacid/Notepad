#ifndef _SHIFTDOWNACTION_H //guard
#define _SHIFTDOWNACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftDownAction : public KeyAction {
public:
	ShiftDownAction(CWnd* parent);
	virtual ~ShiftDownAction();

	virtual void Perform();
};

#endif // !_SHIFTDOWNACTION_H
