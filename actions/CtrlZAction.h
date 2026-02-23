#ifndef _CTRLZACTION_H //guard
#define _CTRLZACTION_H
#include "Action.h"

class CtrlZAction : public Action {
public:
	CtrlZAction(CWnd* parent);
	virtual ~CtrlZAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CtrlZAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CTRLZACTION_H
