#ifndef _CTRLFACTION_H //guard
#define _CTRLFACTION_H
#include "Action.h"

class CWnd;

class CtrlFAction : public Action {
public:
	CtrlFAction(CWnd* parent);
	virtual ~CtrlFAction();

	virtual void Perform();
};

#endif // !_CTRLFACTION_H
