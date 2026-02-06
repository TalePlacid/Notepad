#ifndef _MOUSEACTION_H
#define _MOUSEACTION_H
#include "Action.h"

class CWnd;

class MouseAction : public Action {
public:
	MouseAction(CWnd* parent, CPoint point);
	virtual ~MouseAction() = 0;
protected:
	CPoint point;
};

#endif // !_MOUSEACTION_H
