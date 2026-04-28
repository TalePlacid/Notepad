#ifndef _SELECTPAGEUPACTION_H //guard
#define _SELECTPAGEUPACTION_H
#include "Action.h"

class CWnd;

class SelectPageUpAction : public Action {
public:
	SelectPageUpAction(CWnd* parent);
	virtual ~SelectPageUpAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectPageUpAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTPAGEUPACTION_H

