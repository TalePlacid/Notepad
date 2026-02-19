#ifndef _SELECTPAGEDOWNACTION_H //guard
#define _SELECTPAGEDOWNACTION_H
#include "Action.h"

class CWnd;

class SelectPageDownAction : public Action {
public:
	SelectPageDownAction(CWnd* parent);
	virtual ~SelectPageDownAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectPageDownAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTPAGEDOWNACTION_H
