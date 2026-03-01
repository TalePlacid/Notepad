#ifndef _COPYACTION_H //guard
#define _COPYACTION_H
#include "Action.h"

class CWnd;

class CopyAction : public Action {
public:
	CopyAction(CWnd* parent);
	virtual ~CopyAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool CopyAction::ShouldKeepSelection() {
	return true;
}

#endif // !_COPYACTION_H

