#ifndef _MACROCOMMAND_H //guard
#define _MACROCOMMAND_H
#include "Command.h"
#include "Array.h"

typedef signed long int Long;

class CWnd;

class MacroCommand : public Command {
public:
	MacroCommand(CWnd* parent, Long capacity = 64);
	virtual ~MacroCommand();

	virtual void Execute();
	virtual void Undo();
	Long Add(Command* command);

protected:
	Array<Command*> commands;
	Long capacity;
	Long length;
};

#endif // !_MACROCOMMAND_H
