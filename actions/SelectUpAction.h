#ifndef _SELECTUPACTION_H //guard
#define _SELECTUPACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectUpAction : public Action {
public:
	SelectUpAction(CWnd* parent);
	virtual ~SelectUpAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectUpAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTUPACTION_H
