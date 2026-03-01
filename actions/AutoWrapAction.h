#ifndef _AUTOWRAPACTION_H //guard
#define _AUTOWRAPACTION_H
#include "Action.h"

class AutoWrapAction : public Action {
public:
	AutoWrapAction(CWnd* parent);
	virtual ~AutoWrapAction();

	virtual void Perform();
};

#endif // !_AUTOWRAPACTION_H
