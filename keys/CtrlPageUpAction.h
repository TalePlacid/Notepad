#ifndef _CTRLPAGEUPACTION_H
#define _CTRLPAGEUPACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlPageUpAction : public KeyAction {
public:
	CtrlPageUpAction(CWnd* parent);
	virtual ~CtrlPageUpAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

inline bool CtrlPageUpAction::NeedScrollBarUpdate() {
	return false;
}

#endif // !_CTRLPAGEUPACTION_H
