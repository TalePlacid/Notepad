#ifndef _HSCROLLPAGERIGHTACTION_H //guard
#define _HSCROLLPAGERIGHTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class HScrollPageRightAction : public Action {
public:
	HScrollPageRightAction(CWnd* parent);
	virtual ~HScrollPageRightAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool ShouldKeepSelection();
};

#endif // !_HSCROLLPAGERIGHTACTION_H
