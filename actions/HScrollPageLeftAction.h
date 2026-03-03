#ifndef _HSCROLLPAGELEFTACTION_H //guard
#define _HSCROLLPAGELEFTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class HScrollPageLeftAction : public Action {
public:
	HScrollPageLeftAction(CWnd* parent);
	virtual ~HScrollPageLeftAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
};

#endif // !_HSCROLLPAGELEFTACTION_H
