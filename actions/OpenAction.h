#ifndef _OPENACTION_H //guard
#define _OPENACTION_H
#include "Action.h"

class CWnd;

class OpenAction : public Action {
public:
	OpenAction(CWnd* parent);
	virtual ~OpenAction();

	virtual void Perform();
	virtual bool NeedUpdateCaption();
};

inline bool OpenAction::NeedUpdateCaption() {
	return true;
}

#endif // !_OPENACTION_H

