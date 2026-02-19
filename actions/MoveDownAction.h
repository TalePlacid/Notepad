#ifndef _MOVEDOWNACTION_H //guard
#define _MOVEDOWNACTION_H
#include "Action.h"

class MoveDownAction : public Action {
public:
	MoveDownAction(CWnd* parent);
	virtual ~MoveDownAction();

	virtual void Perform();
};

#endif // !_MOVEDOWNACTION_H
