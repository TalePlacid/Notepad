#ifndef _HSCROLLBARDRAGACTION_H //guard
#define _HSCROLLBARDRAGACTION_H
#include "ScrollBarAction.h"

class Cwnd;

class HScrollBarDragAction : public ScrollBarAction {
public:
	HScrollBarDragAction(CWnd* parent, int nPos);
	virtual ~HScrollBarDragAction();

	virtual void Perform();
private:
	int nPos;
};

#endif // !_HSCROLLBARDRAGACTION_H
