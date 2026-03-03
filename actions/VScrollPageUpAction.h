#ifndef _VSCROLLPAGEUPACTION_H //guard
#define _VSCROLLPAGEUPACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class VScrollPageUpAction : public Action {
public:
	VScrollPageUpAction(CWnd* parent);
	virtual ~VScrollPageUpAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

#endif // !_VSCROLLPAGEUPACTION_H
