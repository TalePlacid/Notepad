#ifndef _NEWCOMMAND_H //guard
#define _NEWCOMMAND_H
#include "Command.h"

class NewCommand : public Command {
public:
	NewCommand(CWnd* parent);
	virtual ~NewCommand();

	virtual void Execute();
};

#endif // !_NEWCOMMAND_H
