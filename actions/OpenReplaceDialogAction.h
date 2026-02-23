#ifndef _OPENREPLACEDIALOGCOMMAND_H
#define _OPENREPLACEDIALOGCOMMAND_H
#include "Action.h"

class CWnd;

class OpenReplaceDialogAction : public Action {
public:
	OpenReplaceDialogAction(CWnd* parent);
	virtual ~OpenReplaceDialogAction();

	virtual void Perform();
};

#endif // !_OPENREPLACEDIALOGCOMMAND_H
