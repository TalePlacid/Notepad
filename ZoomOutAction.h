#ifndef _ZOOMOUTACTION_H //guard
#define _ZOOMOUTACTION_H
#include "KeyAction.h"

class ZoomOutAction : public KeyAction {
public:
	ZoomOutAction(CWnd* parent);
	virtual ~ZoomOutAction();

	virtual void Perform();
};

#endif // !_ZOOMOUTACTION_H
