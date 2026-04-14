#ifndef _PRINTACTION_H //guard
#define _PRINTACTION_H
#include "Action.h"

class CWnd;

class PrintAction : public Action {
public:
	PrintAction(CWnd* parent);
	virtual ~PrintAction();

	virtual void Perform();
	virtual bool NeedInProgressCaption();
};

inline bool PrintAction::NeedInProgressCaption() {
	return true;
}

#endif // !_PRINTACTION_H

