#ifndef _NEWCOMMAND_H //guard
#define _NEWCOMMAND_H
#include "Command.h"

class NewCommand : public Command {
public:
	NewCommand(CWnd* parent);
	virtual ~NewCommand();
	NewCommand(const NewCommand& source);
	NewCommand& operator=(const NewCommand& source);

	virtual void Execute();
	virtual void Unexecute();
};

#endif // !_NEWCOMMAND_H
