#ifndef _SELECTLINEFRONTACTION_H //guard
#define _SELECTLINEFRONTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectLineFrontAction : public Action {
public:
	SelectLineFrontAction(CWnd* parent);
	virtual ~SelectLineFrontAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectLineFrontAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTLINEFRONTACTION_H

