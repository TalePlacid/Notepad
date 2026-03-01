#ifndef _MOVEPAGEDOWNACTION_H //guard
#define _MOVEPAGEDOWNACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class MovePageDownAction : public Action {
public:
	MovePageDownAction(CWnd* parent);
	virtual ~MovePageDownAction();

	virtual void Perform();
};

#endif // !_MOVEPAGEDOWNACTION_H

