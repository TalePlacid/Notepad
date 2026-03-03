#ifndef _VSCROLLLINEDOWNACTION_H //guard
#define _VSCROLLLINEDOWNACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class VScrollLineDownAction : public Action {
public:
	VScrollLineDownAction(CWnd* parent);
	virtual ~VScrollLineDownAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

#endif // !_VSCROLLLINEDOWNACTION_H
