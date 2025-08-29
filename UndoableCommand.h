#ifndef _UNDOABLECOMMAND_H
#define _UNDOABLECOMMAND_H
#include <afxwin.h>
#include "Command.h"

class CWnd;

class UndoableCommand :public Command {
public:
	UndoableCommand(CWnd* parent);
	virtual ~UndoableCommand() = 0;

	virtual void Undo() = 0;
	virtual Command* Clone() = 0;
	virtual bool IsUndoable();
	virtual UINT GetId() = 0;
};

#endif // !_UNDOABLECOMMAND_H
