#ifndef _VSCROLLBARUPCLICKACTION_H //guard
#define _VSCROLLBARUPCLICKACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class VScrollBarUpClickAction : public ScrollBarAction {
public:
	VScrollBarUpClickAction(CWnd* parent);
	virtual ~VScrollBarUpClickAction();

	virtual void Perform();
};

#endif // !_VSCROLLBARUPCLICKACTION_H

