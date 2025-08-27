#ifndef _CTRLZACTION_H //guard
#define _CTRLZACTION_H
#include "KeyAction.h"

class CtrlZAction : public KeyAction {
public:
	CtrlZAction(CWnd* parent);
	virtual ~CtrlZAction();

	virtual void Perform();
};

#endif // !_CTRLZACTION_H
