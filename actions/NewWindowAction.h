#ifndef _NEWWINDOWACTION_H //guard
#define _NEWWINDOWACTION_H
#include "Action.h"

class CWnd;

class NewWindowAction : public Action {
public:
	NewWindowAction(CWnd* parent);
	virtual ~NewWindowAction();

	virtual void Perform();
};

#endif // !_NEWWINDOW_H
