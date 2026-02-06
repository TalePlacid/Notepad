#ifndef _DRAGLEFTACTION_H //guard
#define _DRAGLEFTACTION_H
#include <afxwin.h>
#include "MouseAction.h"

class DragLeftAction : public MouseAction {
public:
	DragLeftAction(CWnd* parent, CPoint point);
	virtual ~DragLeftAction();

	virtual void Perform();
};

#endif //!_DRAGLEFTACTION_H