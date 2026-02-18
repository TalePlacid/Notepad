#ifndef _CTRLPAGEDOWNACTION_H //guard
#define _CTRLPAGEDOWNACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlPageDownAction : public KeyAction {
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
