#ifndef _DOWNARROWACTIONFIXED_H //guard
#define _DOWNARROWACTIONFIXED_H
#include "KeyAction.h"

class DownArrowAction_Fixed : public KeyAction {
public:
	DownArrowAction_Fixed(CWnd* parent);
	virtual ~DownArrowAction_Fixed();

	virtual void Perform();
};

#endif // !_DOWNARROWACTIONFIXED_H
