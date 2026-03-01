#ifndef _SELECTWORDRIGHTACTION_H //guard
#define _SELECTWORDRIGHTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectWordRightAction : public Action {
public:
	SelectWordRightAction(CWnd* parent);
	virtual ~SelectWordRightAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectWordRightAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTWORDRIGHTACTION_H

