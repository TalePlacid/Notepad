#include <afxwin.h>
#include "CutCommand.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../ClipboardController.h"
#include "../Editor.h"

CutCommand::CutCommand(CWnd* parent)
	:Command(parent), contents("") {
	this->frontOffset = -1;
	this->rearOffset = -1;
	this->columnIndex = 0;
}

CutCommand::~CutCommand() {

}

CutCommand::CutCommand(const CutCommand& source)
	:Command(source), contents(source.contents) {
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
}

CutCommand& CutCommand::operator=(const CutCommand& source) {
	Command::operator=(source);

	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
	this->contents = source.contents;

	return *this;
}

void CutCommand::Execute() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		ClipboardController* clipboardController = ((NotepadForm*)(this->parent))->clipboardController;
		clipboardController->Copy();

		Editor editor(this->parent);
		this->isUndoable = editor.GetSelectedRange(this->frontOffset, this->rearOffset);
		this->contents = pagingBuffer->MakeSelectedString();
		editor.EraseRange(this->frontOffset, this->rearOffset, this->columnIndex);
	}
}

void CutCommand::Undo() {
	Editor editor(this->parent);
	editor.InsertTextAt(this->frontOffset, this->columnIndex, this->contents, true);
}

void CutCommand::Redo() {
	Editor editor(this->parent);
	editor.EraseRange(this->frontOffset, this->rearOffset, this->columnIndex);
}