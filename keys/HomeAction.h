#ifndef _HOMEACTION_H
#define _HOMEACTION_H
#include "KeyAction.h"

class CWnd;

class HomeAction : public KeyAction {
public:
	HomeAction(CWnd* parent);
	virtual ~HomeAction();

	virtual void Perform();
};

#endif // !_HOMEACTION_H
