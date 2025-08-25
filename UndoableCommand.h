#ifndef _UNDOABLECOMMAND_H
#define _UNDOABLECOMMAND_H
#include "Command.h"

class CWnd;

class UndoableCommand :public Command {
public:
	UndoableCommand(CWnd* parent);
	virtual ~UndoableCommand() = 0;

	virtual void Undo() = 0;
	virtual bool IsUndoable();
};

#endif // !_UNDOABLECOMMAND_H
