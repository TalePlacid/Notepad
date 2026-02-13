#ifndef _MOUSELEFTDOWNACTION_H
#define _MOUSELEFTDOWNACTION_H
#include <afxwin.h>
#include "MouseAction.h"

class MouseLeftDownAction : public MouseAction {
public:
	MouseLeftDownAction(CWnd* parent, CPoint point);
	virtual ~MouseLeftDownAction();

	virtual void Perform();
	virtual bool NeedUpdateLatest();
};

inline bool MouseLeftDownAction::NeedUpdateLatest() {
	return true;
}

#endif // !_MOUSELEFTDOWNACTION_H
