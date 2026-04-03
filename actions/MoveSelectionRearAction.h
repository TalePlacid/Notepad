#ifndef _MOVE_SELECTION_REAR_ACTION_H //guard
#define _MOVE_SELECTION_REAR_ACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class MoveSelectionRearAction : public Action {
public:
	MoveSelectionRearAction(CWnd* parent);
	virtual ~MoveSelectionRearAction();

	virtual void Perform();
};

#endif // !_MOVE_SELECTION_REAR_ACTION_H
