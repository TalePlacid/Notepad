#ifndef _HSCROLLBARPAGELEFTACTION_H //guard
#define _HSCROLLBARPAGELEFTACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class HScrollBarPageLeftAction : public ScrollBarAction {
public:
	HScrollBarPageLeftAction(CWnd* parent);
	virtual ~HScrollBarPageLeftAction();

	virtual void Perform();
};

#endif // !_HSCROLLBARPAGELEFTACTION_H
