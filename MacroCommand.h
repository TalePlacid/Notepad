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
	MacroCommand(const MacroCommand& source);
	MacroCommand& operator=(const MacroCommand& source);

	virtual void Execute();
	virtual void Undo();
	Long Add(Command* command);
	Command* GetAt(Long index);

	Long GetCapacity() const;
	Long GetLength() const;

	Command* operator[](Long index);
protected:
	Array<Command*> commands;
	Long capacity;
	Long length;
};

inline Long MacroCommand::GetCapacity() const {
	return this->capacity;
}

inline Long MacroCommand::GetLength() const {
	return this->length;
}

#endif // !_MACROCOMMAND_H
