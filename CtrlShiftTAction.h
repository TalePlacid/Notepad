#ifndef _CTRLSHIFTTACTION_H //guard
#define _CTRLSHIFTTACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlShiftTAction : public KeyAction {
public:
	CtrlShiftTAction(CWnd* parent);
	virtual ~CtrlShiftTAction();

	virtual void Perform();
};

#endif // !_SHIFTTACTION_H
