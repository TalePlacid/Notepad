#ifndef _SELECTRIGHTACTION_H //guard
#define _SELECTRIGHTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectRightAction : public Action {
public:
	SelectRightAction(CWnd* parent);
	virtual ~SelectRightAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectRightAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTRIGHTACTION_H

