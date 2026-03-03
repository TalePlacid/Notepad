#ifndef _HSCROLLLINELEFTACTION_H //guard
#define _HSCROLLLINELEFTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class HScrollLineLeftAction : public Action {
public:
	HScrollLineLeftAction(CWnd* parent);
	virtual ~HScrollLineLeftAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
};

#endif // !_HSCROLLLINELEFTACTION_H
