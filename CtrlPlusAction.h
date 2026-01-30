#ifndef _CTRLPLUSACTION_H
#define _CTRLPLUSACTION_H
#include "KeyAction.h"

class CtrlPlusAction : public KeyAction {
public:
	CtrlPlusAction(CWnd* parent);
	virtual ~CtrlPlusAction();

	virtual void Perform();
};

#endif // !_CTRLPLUSACTION_H
