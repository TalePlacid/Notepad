#ifndef _SHIFTENDACTION_H //guard
#define _SHIFTENDACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftEndAction : public KeyAction {
public:
	ShiftEndAction(CWnd* parent);
	virtual ~ShiftEndAction();

	virtual void Perform();
};

#endif // !_SHIFTENDACTION_H
