#ifndef _MOVEWORDLEFTACTION_H //guard
#define _MOVEWORDLEFTACTION_H
#include "KeyAction.h"

class MoveWordLeftAction : public Action {
public:
	MoveWordLeftAction(CWnd* parent);
	virtual ~MoveWordLeftAction();

	virtual void Perform();
};

#endif // !_MOVEWORDLEFTACTION_H
