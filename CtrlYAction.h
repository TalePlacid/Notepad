#ifndef _CTRLYACTION_H //guard
#define _CTRLYACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlYAction : public KeyAction {
public:
	CtrlYAction(CWnd* parent);
	virtual ~CtrlYAction();

	virtual void Perform();
};

#endif // !_CTRLYACTION_H
