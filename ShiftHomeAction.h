#ifndef _SHIFTHOMEACTION_H //guard
#define _SHIFTHOMEACTION_H
#include "KeyAction.h"

typedef signed long int Long;

class CWnd;

class ShiftHomeAction : public KeyAction {
public:
	ShiftHomeAction(CWnd* parent);
	virtual ~ShiftHomeAction();

	virtual void Perform();
};

#endif // !_SHIFTHOMEACTION_H
