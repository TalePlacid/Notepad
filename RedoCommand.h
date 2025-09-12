#ifndef _REDOCOMMAND_H //guard
#define _REDOCOMMAND_H
#include "Command.h"

class CWnd;

class RedoCommand : public Command {
public:
	RedoCommand(CWnd* parent);
	virtual ~RedoCommand();

	virtual void Execute();
};

#endif // !_REDOCOMMAND_H
