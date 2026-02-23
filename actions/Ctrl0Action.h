#ifndef _CTRL0ACTION_H //guard
#define _CTRL0ACTION_H
#include "Action.h"

class Ctrl0Action : public KeyAction {
public:
	Ctrl0Action(CWnd* parent);
	virtual ~Ctrl0Action();

	virtual void Perform();
};

#endif // !_CTRL0ACTION_H
