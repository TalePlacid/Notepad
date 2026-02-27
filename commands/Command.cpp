#include "Command.h"

#pragma warning(disable:4996)

Command::Command(CWnd* parent) {
	this->parent = parent;
	this->isUndoable = false;
}

Command::~Command() {

}

Command::Command(const Command& source) {
	this->parent = source.parent;
	this->isUndoable = source.isUndoable;
}

Command& Command::operator=(const Command& source) {
	this->parent = source.parent;
	this->isUndoable = source.isUndoable;

	return *this;
}

void Command::Undo() {

}

void Command::Redo() {

}

Command* Command::Clone() {
	return 0;
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

AppID Command::GetID() {
	return AppID::NONE;
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