#ifndef _VSCROLLBARPAGEUPACTION_H //guard
#define _VSCROLLBARPAGEUPACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class VScrollBarPageUpAction : public ScrollBarAction {
public:
	VScrollBarPageUpAction(CWnd* parent);
	virtual ~VScrollBarPageUpAction();

	virtual void Perform();
};

#endif // !_VSCROLLBARPAGEUPACTION_H

