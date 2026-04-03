#ifndef _MOVE_SELECTION_FRONT_ACTION_H //guard
#define _MOVE_SELECTION_FRONT_ACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class MoveSelectionFrontAction : public Action {
public:
	MoveSelectionFrontAction(CWnd* parent);
	virtual ~MoveSelectionFrontAction();

	virtual void Perform();
};

#endif // !_MOVE_SELECTION_FRONT_ACTION_H
