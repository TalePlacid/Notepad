#ifndef _CTRLSHIFTHOMEACTION_H //guard
#define _CTRLSHIFTHOMEACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class CtrlShiftHomeAction : public KeyAction {
public:
	CtrlShiftHomeAction(CWnd* parent);
	virtual ~CtrlShiftHomeAction();

	virtual void Perform();
};

#endif // !_CTRLSHIFTHOMEACTION_H
