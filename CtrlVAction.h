#ifndef _CTRLVACTION_H //guard
#define _CTRLVACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlVAction : public KeyAction {
public:
	CtrlVAction(CWnd* parent);
	virtual ~CtrlVAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CtrlVAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CTRLVACTION_H
