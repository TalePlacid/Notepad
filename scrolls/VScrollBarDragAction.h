#ifndef _VSCROLLBARDRAGACTION_H //guard
#define _VSCROLLBARDRAGACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class VScrollBarDragAction : public ScrollBarAction {
public:
	VScrollBarDragAction(CWnd* parent, int nPos);
	virtual ~VScrollBarDragAction();

	virtual void Perform();
private:
	int nPos;
};

#endif // !_VSCROLLBARDRAGACTION_H

