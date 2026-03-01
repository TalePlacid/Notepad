#ifndef _MOVEPAGEUPACTION_H //guard
#define _MOVEPAGEUPACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class MovePageUpAction : public Action {
public:
	MovePageUpAction(CWnd* parent);
	virtual ~MovePageUpAction();

	virtual void Perform();
};

#endif // !_MOVEPAGEUPACTION_H

