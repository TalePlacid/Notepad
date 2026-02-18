#ifndef _CTRLYACTION_H //guard
#define _CTRLYACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlYAction : public KeyAction {
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
