#include <afxwin.h>
#include "CutCommand.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../ClipboardController.h"

CutCommand::CutCommand(CWnd* parent)
	:Command(parent), eraseRangeCommand(this->parent) {
	this->isExecuted = false;
}

CutCommand::~CutCommand() {

}

void CutCommand::Execute() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		ClipboardController* clipboardController = ((NotepadForm*)(this->parent))->clipboardController;
		clipboardController->Copy();

		this->eraseRangeCommand.Execute();
		this->isExecuted = true;
	}
}

void CutCommand::Undo() {
	if (this->isExecuted)
	{
		this->eraseRangeCommand.Undo();
	}
}

void CutCommand::Redo() {
	if (this->isExecuted)
	{
		this->eraseRangeCommand.Redo();
	}
}

AppID CutCommand::GetID() {
	return AppID::ID_COMMAND_CUT;
}