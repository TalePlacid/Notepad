#ifndef _SHIFTRIGHTACTION_H //guard
#define _SHIFTRIGHTACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftRightAction : public KeyAction {
public:
	ShiftRightAction(CWnd* parent);
	virtual ~ShiftRightAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ShiftRightAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SHIFTRIGHTACTION_H
