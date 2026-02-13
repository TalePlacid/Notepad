#ifndef _UNDOCOMMAND_H //guard
#define _UNDOCOMMAND_H
#include "Command.h"

class CWnd;

class UndoCommand : public Command {
public:
	UndoCommand(CWnd* parent);
	virtual ~UndoCommand();

	virtual void Execute();
};

#endif // !_UNDOCOMMAND_H
