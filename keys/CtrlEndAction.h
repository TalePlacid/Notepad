#ifndef _CTRLENDACTION_H //guard
#define _CTRLENDACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlEndAction : public KeyAction {
public:
	CtrlEndAction(CWnd* parent);
	virtual ~CtrlEndAction();

	virtual void Perform();
};

#endif // !_CTRLENDACTION_H
