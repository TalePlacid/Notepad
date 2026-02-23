#ifndef _CTRLYACTION_H //guard
#define _CTRLYACTION_H
#include "Action.h"

class CWnd;

class CtrlYAction : public Action {
public:
	CtrlYAction(CWnd* parent);
	virtual ~CtrlYAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CtrlYAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CTRLYACTION_H
