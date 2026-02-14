#ifndef _ENDACTION_H //guard
#define _ENDACTION_H
#include "KeyAction.h"

class CWnd;

class EndAction : public KeyAction {
public:
	EndAction(CWnd* parent);
	virtual ~EndAction();

	virtual void Perform();
};

#endif // !_ENDACTION_H
