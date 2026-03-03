#ifndef _HSCROLLLINERIGHTACTION_H //guard
#define _HSCROLLLINERIGHTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class HScrollLineRightAction : public Action {
public:
	HScrollLineRightAction(CWnd* parent);
	virtual ~HScrollLineRightAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
};

#endif // !_HSCROLLLINERIGHTACTION_H
