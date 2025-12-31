#ifndef _CTRLAACTION_H //guard
#define _CTRLAACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlAAction : public KeyAction {
public:
	CtrlAAction(CWnd* parent);
	virtual ~CtrlAAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CtrlAAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CTRLAACTION_H
