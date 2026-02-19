#ifndef _MOVERIGHTACTION_H //guard
#define _MOVERIGHTACTION_H
#include "Action.h"

class CWnd;

class MoveRightAction : public Action {
public:
	MoveRightAction(CWnd* parent);
	virtual ~MoveRightAction();

	virtual void Perform();
};

#endif // !_MOVERIGHTACTION_H
