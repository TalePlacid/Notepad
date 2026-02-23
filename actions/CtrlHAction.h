#ifndef _CTRLHACTION_H //guard
#define _CTRLHACTION_H
#include "Action.h"

class CWnd;

class CtrlHAction : public Action {
public:
	CtrlHAction(CWnd* parent);
	virtual ~CtrlHAction();

	virtual void Perform();
};

#endif // !_CTRLHACTION_H
