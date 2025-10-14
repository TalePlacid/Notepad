#ifndef _LOADCOMMAND_H //guard
#define _LOADCOMMAND_H
#include "Command.h"

class CWnd;

class LoadCommand : public Command {
public:
	LoadCommand(CWnd* parent);
	virtual ~LoadCommand();

	virtual void Execute();
};

#endif // !_LOADCOMMAND_H
