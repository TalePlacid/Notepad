#ifndef _LOADLASTCOMMAND_H //guard
#define _LOADLASTCOMMAND_H
#include "Command.h"

class CWnd;

class LoadLastCommand : public Command {
public:
	LoadLastCommand(CWnd* parent);
	virtual ~LoadLastCommand();

	virtual void Execute();
};

#endif // !_LOADLASTCOMMAND_H
