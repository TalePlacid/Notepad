#ifndef _MOVEPAGERIGHTACTION_H //guard
#define _MOVEPAGERIGHTACTION_H
#include "Action.h"

class CWnd;

class MovePageRightAction : public Action {
public:
	MovePageRightAction(CWnd* parent);
	virtual ~MovePageRightAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

inline bool MovePageRightAction::NeedScrollBarUpdate() {
	return false;
}

#endif // !_MOVEPAGERIGHTACTION_H
