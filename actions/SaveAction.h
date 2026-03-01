#ifndef _SAVEACTION_H //guard
#define _SAVEACTION_H
#include "Action.h"

class CWnd;

class SaveAction : public Action {
public:
	SaveAction(CWnd* parent);
	virtual ~SaveAction();

	virtual void Perform();
};


#endif // !_SAVEACTION_H

