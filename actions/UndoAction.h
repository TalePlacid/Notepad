#ifndef _UNDOACTION_H //guard
#define _UNDOACTION_H
#include "Action.h"

class CWnd;

class UndoAction : public Action {
public:
	UndoAction(CWnd* parent);
	virtual ~UndoAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool UndoAction::ShouldKeepSelection() {
	return true;
}

#endif // !_UNDOACTION_H
