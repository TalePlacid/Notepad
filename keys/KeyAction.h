#ifndef _KEYACTION_H //guard
#define _KEYACTION_H
#include "../Action.h"

class CWnd;

class KeyAction : public Action {
public:
	KeyAction(CWnd* parent);
	virtual ~KeyAction() = 0;
};

#endif // !_KEYDOWNACTION_H
