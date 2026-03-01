#ifndef _FINDACTION_H //guard
#define _FINDACTION_H
#include "Action.h"
#include "../FindReplaceOption.h"

class CWnd;

class FindAction : public Action {
public:
	FindAction(CWnd* parent, FindReplaceOption findReplaceOption);
	virtual ~FindAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
private:
	FindReplaceOption findReplaceOption;
};

inline bool FindAction::ShouldKeepSelection() {
	return true;
}

#endif // !_FINDACTION_H

