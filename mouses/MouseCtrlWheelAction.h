#ifndef _MOUSECTRLWHEELACTION_H
#define _MOUSECTRLWHEELACTION_H
#define DELTA_PER_TICK 120
#include <afxwin.h>
#include "MouseAction.h"

typedef signed long int Long;

class MouseCtrlWheelAction : public MouseAction {
public:
	MouseCtrlWheelAction(CWnd* parent, CPoint point, short zDelta);
	virtual ~MouseCtrlWheelAction();

	virtual void Perform();
private:
	short zDelta;
};

#endif // !_MOUSECTRLWHEELACTION_H

