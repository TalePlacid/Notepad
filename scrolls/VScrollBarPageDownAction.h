#ifndef _VSCROLLBARPAGEDOWNACTION_H //guard
#define _VSCROLLBARPAGEDOWNACTION_H
#include "ScrollBarAction.h"

class CWnd;

class VScrollBarPageDownAction : public ScrollBarAction {
public:
	VScrollBarPageDownAction(CWnd* parent);
	virtual ~VScrollBarPageDownAction();

	virtual void Perform();
};

#endif // !_VSCROLLBARPAGEDOWNACTION_H

