#ifndef _ZOOMINACTION_H //guard
#define _ZOOMINACTION_H
#include "Action.h"

class CWnd;

class ZoomInAction : public Action {
public:
	ZoomInAction(CWnd* parent);
	~ZoomInAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool ZoomInAction::ShouldKeepSelection() {
	return true;
}

#endif // !_ZOOMINACTION_H
