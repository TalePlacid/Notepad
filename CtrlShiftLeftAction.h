#ifndef _CTRLSHIFTLEFTACTION_H //guard
#define _CTRLSHIFTLEFTACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class CtrlShiftLeftAction : public KeyAction {
public:
	CtrlShiftLeftAction(CWnd* parent);
	virtual ~CtrlShiftLeftAction();

	virtual void Perform();
};


#endif // !_CTRLSHIFTLEFTACTION_H
