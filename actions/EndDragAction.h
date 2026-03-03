#ifndef _ENDDRAGACTION_H //guard
#define _ENDDRAGACTION_H
#include "Action.h"

class CWnd;

class EndDragAction : public Action {
public:
	EndDragAction(CWnd* parent);
	virtual ~EndDragAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool EndDragAction::ShouldKeepSelection() {
	return true;
}

#endif // !_ENDDRAGACTION_H
