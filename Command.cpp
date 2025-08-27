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

Command* Command::Clone() {
	return 0;
}

bool Command::IsUndoable() {
	return false;
}