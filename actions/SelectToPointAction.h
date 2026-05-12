#ifndef _SELECTTOPOINTACTION_H
#define _SELECTTOPOINTACTION_H
#include <afxwin.h>
#include "Action.h"

class SelectToPointAction : public Action {
public:
	SelectToPointAction(CWnd* parent, CPoint point);
	virtual ~SelectToPointAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
private:
	CPoint point;
};

inline bool SelectToPointAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTTOPOINTACTION_H
