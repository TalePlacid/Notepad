#ifndef _CLOSEACTION_H //guard
#define _CLOSEACTION_H
#include "Action.h"

class CWnd;

class CloseAction : public Action {
public:
	CloseAction(CWnd* parent);
	virtual ~CloseAction();

	virtual void Perform();
};

#endif // !_CLOSEACTION_H
