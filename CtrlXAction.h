#ifndef _CTRLXACTION_H //guard
#define _CTRLXACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlXAction : public KeyAction {
public:
	CtrlXAction(CWnd* parent);
	virtual ~CtrlXAction();

	virtual void Perform();
};

#endif // !_CTRLXACTION_H
