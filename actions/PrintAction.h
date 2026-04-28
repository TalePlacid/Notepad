#ifndef _PRINTACTION_H //guard
#define _PRINTACTION_H
#include "Action.h"

class CWnd;

class PrintAction : public Action {
public:
	PrintAction(CWnd* parent);
	virtual ~PrintAction();

	virtual void Perform();
};

#endif // !_PRINTACTION_H

