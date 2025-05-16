#ifndef _LEFTARROWACTION_H //guard
#define _LEFTARROWACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class LeftArrowAction : public KeyAction {
public:
	LeftArrowAction(CWnd* parent);
	virtual ~LeftArrowAction();

	virtual void Perform();
};

#endif // !_LEFTARROWSTRATEGY_H
