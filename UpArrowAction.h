#ifndef _UPARROWACTION_H //guard
#define _UPARROWACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class UpArrowAction : public KeyAction {
public:
	UpArrowAction(CWnd* parent);
	virtual ~UpArrowAction();

	virtual void Perform();
};

#endif // !_UPARROWACTION_H
