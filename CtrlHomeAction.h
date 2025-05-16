#ifndef _CTRLHOMEACTION_H //guard
#define _CTRLHOMEACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlHomeAction : public KeyAction {
public:
	CtrlHomeAction(CWnd* parent);
	virtual ~CtrlHomeAction();

	virtual void Perform();
};


#endif // !_CTRLHOMEACTION_H
