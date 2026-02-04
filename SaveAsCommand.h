#ifndef _SAVEASCOMMAND_H //guard
#define _SAVEASCOMMAND_H
#include "Command.h"

class CWnd;

class SaveAsCommand : public Command {
public:
	SaveAsCommand(CWnd* parent);
	virtual ~SaveAsCommand();

	virtual void Execute();
};

#endif // !_SAVEASCOMMAND_H
