#ifndef _ZOOMINACTION_H
#define _ZOOMINACTION_H
#include "KeyAction.h"

class ZoomInAction : public KeyAction {
public:
	ZoomInAction(CWnd* parent);
	virtual ~ZoomInAction();

	virtual void Perform();
};

#endif // !_ZOOMINACTION_H
