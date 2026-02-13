#ifndef _SHIFTPAGEUPACTION_H //guard
#define _SHIFTPAGEUPACTION_H
#include "KeyAction.h"

class CWnd;

class ShiftPageUpAction : public KeyAction {
public:
	ShiftPageUpAction(CWnd* parent);
	virtual ~ShiftPageUpAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ShiftPageUpAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SHIFTPAGEUPACTION_H
