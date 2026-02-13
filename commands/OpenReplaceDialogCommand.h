#ifndef _OPENREPLACEDIALOGCOMMNAD_H //guard
#define _OPENREPLACEDIALOGCOMMAND_H
#include "Command.h"

class CWnd;

class OpenReplaceDialogCommand : public Command {
public:
	OpenReplaceDialogCommand(CWnd* parent);
	virtual ~OpenReplaceDialogCommand();

	virtual void Execute();
};

#endif // !_OPENREPLACEDIALOGCOMMNAD_H
