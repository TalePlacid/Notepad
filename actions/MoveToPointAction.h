#ifndef _MOVETOPOINTACTION_H //guard
#define _MOVETOPOINTACTION_H
#include <afxwin.h>
#include "Action.h"

class MoveToPointAction : public Action {
public:
	MoveToPointAction(CWnd* parent, CPoint point);
	virtual ~MoveToPointAction();

	virtual void Perform();
	virtual bool NeedUpdateLatestPoint();
private:
	CPoint point;
};

inline bool MoveToPointAction::NeedUpdateLatestPoint() {
	return true;
}

#endif // !_MOVETOPOINTACTION_H
