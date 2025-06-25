#ifndef _PAGEDOWNACTION_H //guard
#define _PAGEDOWNACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class PageDownAction : public KeyAction {
public:
	PageDownAction(CWnd* parent);
	virtual ~PageDownAction();

	virtual void Perform();
};

#endif // !_PAGEDOWNACTION_H
