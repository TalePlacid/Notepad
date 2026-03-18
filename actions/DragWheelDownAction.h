#ifndef _DRAGWHEELDOWNACTION_H //guard
#define _DRAGWHEELDOWNACTION_H
#include <afxwin.h>
#include "Action.h"

class CWnd;

class DragWheelDownAction : public Action {
public:
	DragWheelDownAction(CWnd* parent, CPoint point);
	virtual ~DragWheelDownAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
private:
	CPoint point;
};

inline bool DragWheelDownAction::NeedScrollBarUpdate() {
	return false;
}

inline bool DragWheelDownAction::ShouldKeepSelection() {
	return true;
}

#endif // !_DRAGWHEELDOWNACTION_H