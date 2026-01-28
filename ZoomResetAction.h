#ifndef _ZOOMRESETACTION_H //guard
#define _ZOOMRESETACTION_H
#include "KeyAction.h"

class ZoomResetAction : public KeyAction {
public:
	ZoomResetAction(CWnd* parent);
	virtual ~ZoomResetAction();

	virtual void Perform();
};

#endif // !_ZOOMRESETACTION_H
