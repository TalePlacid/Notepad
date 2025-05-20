#ifndef _HSCROLLBARPAGERIGHTACTION_H //guard
#define _HSCROLLBARPAGERIGHTACTION_H
#include "ScrollBarAction.h"

typedef signed long int Long;

class CWnd;

class HScrollBarPageRightAction : public ScrollBarAction {
public:
	HScrollBarPageRightAction(CWnd* parent);
	virtual ~HScrollBarPageRightAction();

	virtual void Perform();
};

#endif // !_HSCROLLBARPAGERIGHTACTION_H
