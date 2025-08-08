#ifndef _CTRLFACTION_H //guard
#define _CTRLFACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlFAction : public KeyAction {
public:
	CtrlFAction(CWnd* parent);
	virtual ~CtrlFAction();

	virtual void Perform();
};

#endif // !_CTRLFACTION_H
