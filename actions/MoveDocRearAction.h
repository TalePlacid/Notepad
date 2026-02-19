#ifndef _MOVEDOCREARACTION_H //guard
#define _MOVEDOCREARACTION_H
#include "Action.h"

class CWnd;

class MoveDocRearAction : public Action {
public:
	MoveDocRearAction(CWnd* parent);
	virtual ~MoveDocRearAction();

	virtual void Perform();
};

#endif // !_MOVEDOCREARACTION_H
