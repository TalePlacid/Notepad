#ifndef _REDOACTION_H //guard
#define _REDOACTION_H
#include "Action.h"

class CWnd;

class RedoAction : public Action {
public:
	RedoAction(CWnd* parent);
	~RedoAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool RedoAction::ShouldKeepSelection() {
	return true;
}

#endif // !_REDOACTION_H

