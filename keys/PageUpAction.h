#ifndef _PAGEUPACTION_H //guard
#define _PAGEUPACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class PageUpAction : public KeyAction {
public:
	PageUpAction(CWnd* parent);
	virtual ~PageUpAction();

	virtual void Perform();
};

#endif // !_PAGEUPACTION_H
