#ifndef _MOVEWORDRIGHTACTION_H //guard
#define _MOVEWORDRIGHTACTION_H
#include "Action.h"

class MoveWordRightAction : public Action {
public:
	MoveWordRightAction(CWnd* parent);
	virtual ~MoveWordRightAction();

	virtual void Perform();
};

#endif // !_MOVEWORDRIGHTACTION_H
