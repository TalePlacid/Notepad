#ifndef _CTRLRIGHTACTION_H //guard
#define _CTRLRIGHTACTION_H
#include "KeyAction.h"

class CtrlRightAction : public KeyAction {
public:
	CtrlRightAction(CWnd* parent);
	virtual ~CtrlRightAction();

	virtual void Perform();
};

#endif // !_CTRLRIGHTACTION_H
