#include "MacroCommand.h"

#pragma warning(disable:4996)

MacroCommand::MacroCommand(CWnd* parent, Long capacity)
	:Command(parent), commands(capacity) {
	this->capacity = capacity;
	this->length = 0;
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

MacroCommand::MacroCommand(const MacroCommand& source)
	:commands(source.commands) {
	Long i = 0;
	while (i < source.length)
	{
		this->commands[i] = const_cast<MacroCommand&>(source).commands[i]->Clone();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
}

MacroCommand& MacroCommand::operator=(const MacroCommand& source) {
	Long i = 0;
	while (i < this->length)
	{
		if (this->commands[i] != NULL)
		{
			delete this->commands[i];
		}
		i++;
	}

	this->commands = source.commands;

	i = 0;
	while (i < source.length)
	{
		this->commands[i] = const_cast<MacroCommand&>(source).commands[i]->Clone();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
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
	Long i = this->length - 1;
	while (i >= 0)
	{
		this->commands[i]->Undo();
		i--;
	}
}

void MacroCommand::Redo() {
	Long i = 0;
	while (i < this->length)
	{
		this->commands[i]->Redo();
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

bool MacroCommand::IsMacroCommand() {
	return true;
}

Command* MacroCommand::GetAt(Long index) {
	return this->commands.GetAt(index);
}

UINT MacroCommand::GetId() {
	return this->commands[0]->GetId();
}

Command* MacroCommand::operator[](Long index) {
	return this->commands[index];
}