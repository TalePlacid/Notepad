#ifndef _LOADFIRSTCOMMAND_H //guard
#define _LOADFIRSTCOMMAND_H
#include "Command.h"

class CWnd;

class LoadFirstCommand : public Command {
public:
	LoadFirstCommand(CWnd* parent);
	virtual ~LoadFirstCommand();

	virtual void Execute();
};

#endif // !_LOADFIRSTCOMMAND_H
