#ifndef _RIGHTARROWACTION_H //guard
#define _RIGHTARROWACTION_H
#include "KeyAction.h"

class CWnd;

class RightArrowAction : public KeyAction {
public:
	RightArrowAction(CWnd* parent);
	virtual ~RightArrowAction();

	virtual void Perform();
};

#endif // !_RIGHTARROWACTION_H
