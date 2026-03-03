#ifndef _VSCROLLLINEUPACTION_H //guard
#define _VSCROLLLINEUPACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class VScrollLineUpAction : public Action {
public:
	VScrollLineUpAction(CWnd* parent);
	virtual ~VScrollLineUpAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

#endif // !_VSCROLLLINEUPACTION_H
