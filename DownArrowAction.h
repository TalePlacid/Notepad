#ifndef _DOWNARROWACTION_H //guard
#define _DOWNARROWACTION_H
#include "KeyAction.h"

class DownArrowAction : public KeyAction {
public:
	DownArrowAction(CWnd* parent);
	virtual ~DownArrowAction();

	virtual void Perform();
};

#endif // !_DOWNARROWACTION_H
