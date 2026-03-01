#ifndef _GOTOACTION_H //guard
#define _GOTOACTION_H
#include "Action.h"

class CWnd;

class GoToAction : public Action {
public:
	GoToAction(CWnd* parent);
	virtual ~GoToAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

inline bool GoToAction::NeedScrollBarUpdate() {
	return true;
}

#endif // !_GOTOACTION_H
