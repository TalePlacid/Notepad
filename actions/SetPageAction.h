#ifndef _SETPAGEACTION_H //guard
#define _SETPAGEACTION_H
#include "Action.h"

class CWnd;

class SetPageAction : public Action {
public:
	SetPageAction(CWnd* parent);
	virtual ~SetPageAction();

	virtual void Perform();
};

#endif // !_SETPAGEACTION_H
