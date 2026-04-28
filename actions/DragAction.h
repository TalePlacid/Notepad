#ifndef _DRAGACTION_H //guard
#define _DRAGACTION_H
#include <afxwin.h>
#include "Action.h"

class CWnd;

class DragAction : public Action {
public:
	DragAction(CWnd* parent, CPoint point);
	virtual ~DragAction();

	virtual void Perform();
	virtual bool NeedUpdateLatestPoint();
	virtual bool ShouldKeepSelection();
private:
	CPoint point;
	BOOL isMoved;
};

inline bool DragAction::NeedUpdateLatestPoint() {
	return this->isMoved;
}

inline bool DragAction::ShouldKeepSelection() {
	return true;
}

#endif // !_DRAGACTION_H
