#ifndef _BACKSPACEACTION_H //guard
#define _BACKSPACEACTION_H
#include "KeyAction.h"

class CWnd;

class BackspaceAction : public KeyAction {
public:
	BackspaceAction(CWnd* parent);
	virtual ~BackspaceAction();

	virtual void Perform();
};


#endif // !_BACKSPACEACTION_H
