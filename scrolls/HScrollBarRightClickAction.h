#ifndef _HSCROLLBARRIGHTCLICKACTION_H //guard
#define _HSCROLLBARRIGHTCLICKACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class HScrollBarRightClickAction : public ScrollBarAction {
public:
	HScrollBarRightClickAction(CWnd* parent);
	virtual ~HScrollBarRightClickAction();

	virtual void Perform();
};

#endif // !_HSCROLLBARRIGHTCLICKACTION_H

