#ifndef _DRAGUPACTION_H //guard
#define _DRAGUPACTION_H
#include "MouseAction.h"

class DragUpAction : public MouseAction {
public:
	DragUpAction(CWnd* parent, CPoint point);
	virtual ~DragUpAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
	virtual bool NeedUpdateLatest();
};

inline bool DragUpAction::ShouldKeepSelection() {
	return true;
}

inline bool DragUpAction::NeedUpdateLatest() {
	return true;
}

#endif // !_DRAGUPACTION_H\
