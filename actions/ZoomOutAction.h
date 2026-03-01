#ifndef _ZOOMOUTACTION_H //guard
#define _ZOOMOUTACTION_H
#include "ZoomOutAction.h"
#include "Action.h"

class CWnd;

class ZoomOutAction : public Action {
public:
	ZoomOutAction(CWnd* parent);
	virtual ~ZoomOutAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ZoomOutAction::ShouldKeepSelection() {
	return true;
}

#endif // !_ZOOMOUTACTION_H

