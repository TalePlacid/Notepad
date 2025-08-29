#include "UndoableCommand.h"

#pragma warning(disable:4996)

UndoableCommand::UndoableCommand(CWnd* parent)
	:Command(parent) {

}

UndoableCommand::~UndoableCommand() {

}

bool UndoableCommand::IsUndoable() {
	return true;
}