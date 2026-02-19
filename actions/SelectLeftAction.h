#ifndef _SELECTLEFTACTION_H //guard
#define _SELECTLEFTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectLeftAction : public Action {
public:
	SelectLeftAction(CWnd* parent);
	virtual ~SelectLeftAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectLeftAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTLEFTACTION_H
