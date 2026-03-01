#ifndef _TOGGLESTATUSBARACTION_H //guard
#define _TOGGLESTATUSBARACTION_H
#include "Action.h"

class ToggleStatusBarAction : public Action {
public:
	ToggleStatusBarAction(CWnd* parent);
	virtual ~ToggleStatusBarAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
};

inline bool ToggleStatusBarAction::NeedScrollBarUpdate() {
	return false;
}

inline bool ToggleStatusBarAction::ShouldKeepSelection() {
	return true;
}

#endif // !_TOGGLESTATUSBARACTION_H
