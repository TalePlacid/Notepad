#ifndef _MOUSEACTION_H
#define _MOUSEACTION_H
#include "../actions/Action.h"

class CWnd;

class MouseAction : public Action {
public:
	MouseAction(CWnd* parent, CPoint point);
	virtual ~MouseAction() = 0;
	virtual bool NeedUpdateLatest();
protected:
	CPoint point;
};

inline bool MouseAction::NeedUpdateLatest() {
	return false;
}

#endif // !_MOUSEACTION_H
