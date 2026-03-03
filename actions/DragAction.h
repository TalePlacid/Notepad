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
};

inline bool DragAction::NeedUpdateLatestPoint() {
	return true;
}

inline bool DragAction::ShouldKeepSelection() {
	return true;
}

#endif // !_DRAGACTION_H
