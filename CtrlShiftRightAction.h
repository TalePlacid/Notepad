#ifndef _CTRLSHIFTRIGHTACTION_H //guard
#define _CTRLSHIFTRIGHTACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class CtrlShiftRightAction : public KeyAction {
public:
	CtrlShiftRightAction(CWnd* parent);
	virtual ~CtrlShiftRightAction();

	virtual void Perform();
};

#endif // !_CTRLSHIFTRIGHTACTION_H
