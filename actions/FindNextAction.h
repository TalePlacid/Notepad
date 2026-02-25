#ifndef _FINDNEXTACTION_H //guard
#define _FINDNEXTACTION_H
#include "Action.h"
#include "../FindReplaceOption.h"

class CWnd;

class FindNextAction : public Action {
public:
	FindNextAction(CWnd* parent);
	virtual ~FindNextAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool FindNextAction::ShouldKeepSelection() {
	return true;
}

#endif // !_FINDNEXTACTION_H
