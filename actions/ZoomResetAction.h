#ifndef _ZOOMRESETACTION_H
#define _ZOOMRESETACTION_H
#include "Action.h"

class CWnd;

class ZoomResetAction : public Action {
public:
	ZoomResetAction(CWnd* parent);
	virtual ~ZoomResetAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ZoomResetAction::ShouldKeepSelection() {
	return true;
}

#endif // !_ZOOMRESETACTION_H

