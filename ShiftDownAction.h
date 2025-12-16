#ifndef _SHIFTDOWNACTION_H //guard
#define _SHIFTDOWNACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftDownAction : public KeyAction {
public:
	ShiftDownAction(CWnd* parent);
	virtual ~ShiftDownAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ShiftDownAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SHIFTDOWNACTION_H
