#ifndef _DRAGWHEELUPACTION_H //guard
#define _DRAGWHEELUPACTION_H
#include <afxwin.h>
#include "Action.h"

class CWnd;

class DragWheelUpAction : public Action {
public:
	DragWheelUpAction(CWnd* parent, CPoint point);
	virtual ~DragWheelUpAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
private:
	CPoint point;
};

inline bool DragWheelUpAction::NeedScrollBarUpdate() {
	return false;
}

inline bool DragWheelUpAction::ShouldKeepSelection() {
	return true;
}

#endif // !_DRAGWHEELUPACTION_H