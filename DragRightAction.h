#ifndef _DRAGRIGHTACTION_H //guard
#define _DRAGRIGHTACITON_H
#include <afxwin.h>
#include "MouseAction.h"

class DragRightAction : public MouseAction {
public:
	DragRightAction(CWnd* parent, CPoint point);
	virtual ~DragRightAction();

	virtual void Perform();
};

#endif // !_DRAGRIGHTACTION_H
