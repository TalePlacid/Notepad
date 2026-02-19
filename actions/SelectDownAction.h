#ifndef _SELECTDOWNACTION_H //guard
#define _SELECTDOWNACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectDownAction : public Action {
public:
	SelectDownAction(CWnd* parent);
	virtual ~SelectDownAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectDownAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTDOWNACTION_H
