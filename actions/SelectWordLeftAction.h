#ifndef _SELECTWORDLEFTACTION_H //guard
#define _SELECTWORDLEFTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectWordLeftAction : public Action {
public:
	SelectWordLeftAction(CWnd* parent);
	virtual ~SelectWordLeftAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectWordLeftAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTWORDLEFTACTION_H

