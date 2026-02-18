#ifndef _CTRLMINUSACTION_H //guard
#define _CTRLMINUSACTION_H
#include "KeyAction.h"

class CtrlMinusAction : public KeyAction {
public:
	CtrlMinusAction(CWnd* parent);
	virtual ~CtrlMinusAction();

	virtual void Perform();
};

#endif // !_CTRLMINUSACTION_H
