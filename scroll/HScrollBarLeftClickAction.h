#ifndef _HSCROLLBARLEFTCLICKACTION_H //guard
#define _HSCROLLBARLEFTCLICKACTION_H
#include "ScrollBarAction.h"

class CWnd;

class HScrollBarLeftClickAction : public ScrollBarAction {
public:
	HScrollBarLeftClickAction(CWnd* parent);
	virtual ~HScrollBarLeftClickAction();

	virtual void Perform();
};

#endif // !_HSCROLLBARLEFTCLICKACTION_H
