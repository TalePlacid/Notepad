#include <afxwin.h>
#include "CutCommand.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../ClipboardController.h"
#include "../Editor.h"

CutCommand::CutCommand(CWnd* parent)
	:Command(parent) {
	this->isExecuted = false;
	this->columnIndex = 0;
}

CutCommand::~CutCommand() {

}

void CutCommand::Execute() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		ClipboardController* clipboardController = ((NotepadForm*)(this->parent))->clipboardController;
		clipboardController->Copy();

		Editor editor(this->parent);
		Long frontOffset;
		Long rearOffset;
		editor.GetSelectedRange(frontOffset, rearOffset);
		editor.EraseRange(frontOffset, rearOffset, this->columnIndex, this->erased);

		this->isExecuted = true;
	}
}

void CutCommand::Undo() {
}

void CutCommand::Redo() {
}

AppID CutCommand::GetID() {
	return AppID::ID_COMMAND_CUT;
}