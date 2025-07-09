#ifndef _CTRLENDACTIONFIXED_H //guard
#define _CTRLENDACTIONFIXED_H
#include "KeyAction.h"

class CWnd;

class CtrlEndAction_Fixed : public KeyAction {
public:
	CtrlEndAction_Fixed(CWnd* parent);
	virtual ~CtrlEndAction_Fixed();

	virtual void Perform();
};

#endif // !_CTRLENDACTIONFIXED_H
