#ifndef _VSCROLLPAGEDOWNACTION_H //guard
#define _VSCROLLPAGEDOWNACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class VScrollPageDownAction : public Action {
public:
	VScrollPageDownAction(CWnd* parent);
	virtual ~VScrollPageDownAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

#endif // !_VSCROLLPAGEDOWNACTION_H
