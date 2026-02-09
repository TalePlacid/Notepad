#ifndef _MOUSEWHEELACTION_H //guard
#define _MOUSEWHEELACTION_H
#define DELTA_PER_TICK 120
#include <afxwin.h>
#include "MouseAction.h"

class MouseWheelAction : public MouseAction {
public:
	MouseWheelAction(CWnd* parent, CPoint point, short zDelta);
	virtual ~MouseWheelAction();

	virtual void Perform();
private:
	short zDelta;
};

#endif // !_MOUSEWHEELACTION_H
