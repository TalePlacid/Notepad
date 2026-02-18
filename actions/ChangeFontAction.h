#ifndef _CHANGEFONTACTION_H //guard
#define _CHANGEFONTACTION_H
#include "Action.h"

class CWnd;

class ChangeFontAction : public Action {
public:
	ChangeFontAction(CWnd* parent);
	virtual ~ChangeFontAction();

	virtual void Perform();
};

#endif // !_CHANGEFONTACTION_H
