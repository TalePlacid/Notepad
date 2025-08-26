#include <afxwin.h>
#include "MacroCommand.h"

#pragma warning(disable:4996)

MacroCommand::MacroCommand(CWnd* parent, Long capacity)
	:Command(parent), commands(capacity) {
	this->capacity = capacity;
	this->length = length;
}

MacroCommand::~MacroCommand() {
	Long i = 0;
	while (i < this->length)
	{
		if (this->commands[i] != NULL)
		{
			delete this->commands[i];
		}
		i++;
	}
}

void MacroCommand::Execute() {
	Long i = 0;
	while (i < this->length)
	{
		this->commands[i]->Execute();
		i++;
	}
}

void MacroCommand::Undo() {
	Long i = 0;
	while (i < this->length)
	{
		this->commands[i]->Undo();
		i++;
	}
}

Long MacroCommand::Add(Command* command) {
	Long index = -1;
	if (this->length < this->capacity)
	{
		index = this->commands.Store(this->length, command);
	}
	else
	{
		index = this->commands.AppendFromRear(command);
		(this->capacity)++;
	}
	(this->length)++;

	return index;
}