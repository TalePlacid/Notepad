#ifndef _OPENFINDDIALOGACTION_H //guard
#define _OPENFINDDIALOGACTION_H
#include "Action.h"

class CWnd;

class OpenFindDialogAction : public Action {
public:
	OpenFindDialogAction(CWnd* parent);
	virtual ~OpenFindDialogAction();

	virtual void Perform();
};

#endif // !_OPENFINDDIALOGACTION_H
