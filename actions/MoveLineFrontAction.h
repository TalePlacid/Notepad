#ifndef _MOVELINEFRONTACTION_H //guard
#define _MOVELINEFRONTACTION_H
#include "Action.h"

class CWnd;

class MoveLineFrontAction : public Action {
public:
	MoveLineFrontAction(CWnd* parent);
	virtual ~MoveLineFrontAction();

	virtual void Perform();
};

#endif // !_MOVELINEFRONTACTION_H
