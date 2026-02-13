#ifndef _DRAGRIGHTACTION_H //guard
#define _DRAGRIGHTACITON_H
#include <afxwin.h>
#include "MouseAction.h"

class DragRightAction : public MouseAction {
public:
	DragRightAction(CWnd* parent, CPoint point);
	virtual ~DragRightAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
	virtual bool NeedUpdateLatest();
};

inline bool DragRightAction::ShouldKeepSelection() {
	return true;
}

inline bool DragRightAction::NeedUpdateLatest() {
	return true;
}

#endif // !_DRAGRIGHTACTION_H
