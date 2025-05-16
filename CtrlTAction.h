#ifndef _CTRLTACTION_H //guard
#define _CTRLTACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlTAction : public KeyAction {
public:
	CtrlTAction(CWnd* parent);
	virtual ~CtrlTAction();

	virtual void Perform();
};

#endif // !_TESTKEYDOWN_H
