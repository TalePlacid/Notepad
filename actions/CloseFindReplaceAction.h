#ifndef _CLOSEFINDREPLACEDIALOGACTION_H //guard
#define _CLOSEFINDREPLACEDIALOGACTION_H
#include "Action.h"

class CWnd;

class CloseFindReplaceAction : public Action {
public:
	CloseFindReplaceAction(CWnd* parent);
	virtual ~CloseFindReplaceAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CloseFindReplaceAction::ShouldKeepSelection() {
	return true;
}

#endif // !_CLOSEFINDREPLACEDIALOGACTION_H

