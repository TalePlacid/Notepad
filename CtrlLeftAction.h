#ifndef _CTRLLEFTACTION_H //guard
#define _CTRLLEFTACTION_H
#include "KeyAction.h"

class CtrlLeftAction : public KeyAction {
public:
	CtrlLeftAction(CWnd* parent);
	virtual ~CtrlLeftAction();

	virtual void Perform();
};

#endif // !_CTRLLEFTACTION_H
