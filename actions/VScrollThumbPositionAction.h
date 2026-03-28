#ifndef _VSCROLLTHUMBPOSITIONACTION_H //guard
#define _VSCROLLTHUMBPOSITIONACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class VScrollThumbPositionAction : public Action {
public:
	VScrollThumbPositionAction(CWnd* parent);
	virtual ~VScrollThumbPositionAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

#endif // !_VSCROLLTHUMBPOSITIONACTION_H
