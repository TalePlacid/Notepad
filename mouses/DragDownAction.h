#ifndef _DRAGDOWNACTION_H //guard
#define _DRAGDOWNACTION_H
#include <afxwin.h>
#include "MouseAction.h"

class DragDownAction : public MouseAction {
public:
	DragDownAction(CWnd* parent, CPoint point);
	virtual ~DragDownAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
	virtual bool NeedUpdateLatest();
};

inline bool DragDownAction::ShouldKeepSelection() {
	return true;
}

inline bool DragDownAction::NeedUpdateLatest() {
	return true;
}

#endif // !_DRAGDOWNACTION_H

