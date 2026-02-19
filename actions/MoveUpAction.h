#ifndef _MOVEUPACTION_H //guard
#define _MOVEUPACTION_H
#include "Action.h"

typedef signed long int Long;

class MoveUpAction : public Action {
public:
	MoveUpAction(CWnd* parent);
	virtual ~MoveUpAction();

	virtual void Perform();
};

#endif // !_MOVEUPACTION_H
