#ifndef _SAVEASACTION_H
#define _SAVEASACTION_H
#include "Action.h"

class CWnd;

class SaveAsAction : public Action {
public:
	SaveAsAction(CWnd* parent);
	virtual ~SaveAsAction();

	virtual void Perform();
};

#endif // !_SAVEASACTION_H

