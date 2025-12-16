#ifndef _CTRLSHIFTENDACTION_H //guard
#define _CTRLSHIFTENDACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class CtrlShiftEndAction : public KeyAction {
public:
	CtrlShiftEndAction(CWnd* parent);
	virtual ~CtrlShiftEndAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CtrlShiftEndAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CTRLSHIFTENDACTION_H
