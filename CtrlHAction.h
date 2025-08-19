#ifndef _CTRLHACTION_H //guard
#define _CTRLHACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlHAction : public KeyAction {
public:
	CtrlHAction(CWnd* parent);
	virtual ~CtrlHAction();

	virtual void Perform();
};

#endif // !_CTRLHACTION_H
