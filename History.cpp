#include "History.h"

#pragma warning(disable:4996)

History::History(CWnd* parent, Long capacity)
	:MacroCommand(parent, capacity) {

}

History::~History() {

}

History::History(const History& source)
	:MacroCommand(source) {

}

History& History::operator=(const History& source) {
	MacroCommand::operator=(source);

	return *this;
}