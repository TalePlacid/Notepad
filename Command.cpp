#include "Command.h"

#pragma warning(disable:4996)

Command::Command(CWnd* parent) {
	this->parent = parent;
}

Command::~Command() {

}

Command::Command(const Command& source) {
	this->parent = source.parent;
}

Command& Command::operator=(const Command& source) {
	this->parent = source.parent;

	return *this;
}

void Command::Undo() {

}

void Command::Redo() {

}

Command* Command::Clone() {
	return 0;
}

bool Command::IsUndoable() {
	return false;
}

Long Command::Add(Command* command) {
	return -1;
}

Command* Command::GetAt(Long index) {
	return 0;
}

bool Command::IsMacroCommand() {
	return false;
}

UINT Command::GetId() {
	return 0;
}

void Command::Update(Command *command, bool isDone) {

}

Long Command::GetOffset() {
	return -1;
}

CString Command::GetSource() {
	return CString("");
}

CString Command::GetReplaced() {
	return CString("");
}