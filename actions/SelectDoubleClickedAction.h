#ifndef _SELECTDOUBLECLICKEDACTION_H //guard
#define _SELECTDOUBLECLICKEDACTION_H
#include "Action.h"
#include <afxwin.h>

class SelectDoubleClickedAction : public Action {
public:
	SelectDoubleClickedAction(CWnd* parent, CPoint point);
	virtual ~SelectDoubleClickedAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
private:
	CPoint point;
};

inline bool SelectDoubleClickedAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTDOUBLECLICKEDACTION_H
