#ifndef _DRAGUPACTION_H //guard
#define _DRAGUPACTION_H
#include "MouseAction.h"

class DragUpAction : public MouseAction {
public:
	DragUpAction(CWnd* parent, CPoint point);
	virtual ~DragUpAction();

	virtual void Perform();
};

#endif // !_DRAGUPACTION_H\
