#ifndef _CTRLCACTION_H //guard
#define _CTRLCACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlCAction : public KeyAction {
public:
	CtrlCAction(CWnd* parent);
	virtual ~CtrlCAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CtrlCAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CTRLCACTION_H
