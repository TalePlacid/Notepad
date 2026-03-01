#ifndef _MOVEPAGELEFTACTION_H //guard
#define _MOVEPAGELEFTACTION_H
#include "Action.h"

class CWnd;

class MovePageLeftAction : public Action {
public:
	MovePageLeftAction(CWnd* parent);
	virtual ~MovePageLeftAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

inline bool MovePageLeftAction::NeedScrollBarUpdate() {
	return false;
}

#endif // !_MOVEPAGELEFTACTION_H

