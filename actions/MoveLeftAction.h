#ifndef _MOVELEFTACTION_H //guard
#define _MOVELEFTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class MoveLeftAction : public Action {
public:
	MoveLeftAction(CWnd* parent);
	virtual ~MoveLeftAction();

	virtual void Perform();
};

#endif // !_MOVELEFTACTION_H

