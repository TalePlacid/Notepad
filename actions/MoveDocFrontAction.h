#ifndef _MOVEDOCFRONTACTION_H //guard
#define _MOVEDOCFRONTACTION_H
#include "Action.h"

class CWnd;

class MoveDocFrontAction : public Action {
public:
	MoveDocFrontAction(CWnd* parent);
	virtual ~MoveDocFrontAction();

	virtual void Perform();
};


#endif // !_MOVEDOCFRONTACTION_H
