#ifndef _CTRLCACTION_H //guard
#define _CTRLCACTION_H
#include "KeyAction.h"

class CWnd;

class CtrlCAction : public KeyAction {
public:
	CtrlCAction(CWnd* parent);
	virtual ~CtrlCAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection() const;
};

inline bool CtrlCAction::ShouldKeepSelection() const {
	return true;
}

#endif // !_CTRLCACTION_H
