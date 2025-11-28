#ifndef _CTRLSHIFTHOMEACTION_H //guard
#define _CTRLSHIFTHOMEACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class CtrlShiftHomeAction : public KeyAction {
public:
	CtrlShiftHomeAction(CWnd* parent);
	virtual ~CtrlShiftHomeAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection() const;
};

inline bool CtrlShiftHomeAction::ShouldKeepSelection() const {
	return true;
}

#endif // !_CTRLSHIFTHOMEACTION_H
