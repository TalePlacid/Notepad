#ifndef _CUTCOMMAND_H //guard
#define _CUTCOMMAND_H
#include "Command.h"
#include "EraseRangeCommand.h"

class CWnd;

class CutCommand : public Command {
public:
	CutCommand(CWnd* parent);
	virtual ~CutCommand();

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

	virtual AppID GetID();
private:
	EraseRangeCommand eraseRangeCommand;
	bool isExecuted;
};

#endif // !_CUTCOMMAND_H
