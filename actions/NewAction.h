#ifndef _NEWACTION_H
#define _NEWACTION_H
#include "Action.h"

class NewAction : public Action {
public:
	NewAction(CWnd* parent);
	virtual ~NewAction();

	virtual void Perform();
};

#endif // !_NEWACTION_H

