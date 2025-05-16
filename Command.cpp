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

void Command::Unexecute() {

}