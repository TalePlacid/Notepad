#ifndef _VSCROLLBARDOWNCLICKACTION_H //guard
#define _VSCROLLBARDOWNCLICKACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class VScrollBarDownClickAction : public ScrollBarAction {
public:
	VScrollBarDownClickAction(CWnd* parent);
	virtual ~VScrollBarDownClickAction();

	virtual void Perform();
};

#endif // !_VSCROLLBARDOWNCLICKACTION_H

