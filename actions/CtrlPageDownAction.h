#ifndef _CTRLPAGEDOWNACTION_H //guard
#define _CTRLPAGEDOWNACTION_H
#include "Action.h"

class CWnd;

class CtrlPageDownAction : public Action {
public:
	CtrlPageDownAction(CWnd* parent);
	virtual ~CtrlPageDownAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

inline bool CtrlPageDownAction::NeedScrollBarUpdate() {
	return false;
}

#endif // !_CTRLSHIFTPAGEDOWNACTION_H
