#ifndef _SHIFTPAGEDOWNACTION_H //guard
#define _SHIFTPAGEDOWNACTION_H
#include "KeyAction.h"

class CWnd;

class ShiftPageDownAction : public KeyAction {
public:
	ShiftPageDownAction(CWnd* parent);
	virtual ~ShiftPageDownAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ShiftPageDownAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SHIFTPAGEDOWNACTION_H
