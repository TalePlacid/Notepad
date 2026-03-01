#ifndef _SELECTALLACTION_H //guard
#define _SELECTALLACTION_H
#include "Action.h"

class CWnd;

class SelectAllAction : public Action {
public:
	SelectAllAction(CWnd* parent);
	virtual ~SelectAllAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectAllAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTALLACTION_H

