#ifndef _MACROCOMMAND_H //guard
#define _MACROCOMMAND_H
#include <afxwin.h>
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
	virtual Long Add(Command* command);
	virtual Command* GetAt(Long index);
	virtual bool IsMacroCommand();
	virtual UINT GetId();

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
