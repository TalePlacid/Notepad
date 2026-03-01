#ifndef _MOVELINEREARACTION_H //guard
#define _MOVELINEREARACTION_H
#include "Action.h"

class CWnd;

class MoveLineRearAction : public Action {
public:
	MoveLineRearAction(CWnd* parent);
	virtual ~MoveLineRearAction();

	virtual void Perform();
};

#endif // !_MOVELINEREARACTION_H

