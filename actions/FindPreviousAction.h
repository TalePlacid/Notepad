#ifndef _FINDPREVIOUSACTION_H
#define _FINDPREVIOUSACTION_H
#include "Action.h"

class CWnd;

class FindPreviousAction : public Action {
public:
	FindPreviousAction(CWnd* parent);
	virtual ~FindPreviousAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool FindPreviousAction::ShouldKeepSelection() {
	return true;
}

#endif // !_FINDPREVIOUSACTION_H

