#include <afxwin.h>
#include "ReplaceCommand.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"
#include "../Editor.h"

#pragma warning(disable:4996)

ReplaceCommand::ReplaceCommand(CWnd* parent, FindReplaceOption findReplaceOption)
	:Command(parent), source(""), replaced("") {
	this->findReplaceOption = findReplaceOption;
	this->columnIndex = 0;
	this->offset = -1;
}

ReplaceCommand::~ReplaceCommand() {

}

ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	:Command(source) {
	this->findReplaceOption = source.findReplaceOption;
	this->source = source.source;
	this->replaced = source.replaced;
	this->columnIndex = source.columnIndex;
	this->offset = source.offset;
}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	Command::operator=(source);

	this->findReplaceOption = source.findReplaceOption;
	this->source = source.source;
	this->replaced = source.replaced;
	this->columnIndex = source.columnIndex;
	this->offset = source.offset;

	return *this;
}

void ReplaceCommand::Execute() {
	//1. 상태를 적는다.
	this->isUndoable = true;

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	this->offset = pagingBuffer->GetSelectionBeginOffset();
	this->source = pagingBuffer->MakeSelectedString();
	this->replaced = this->findReplaceOption.replaceString;

	//2. 선택을 취소한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 바꾼다.
	Editor editor(this->parent);
	editor.Replace(this->offset, this->source, this->replaced, TRUE, this->columnIndex);

	//4. 다시 찾는다.
	editor.Find(this->findReplaceOption);
}

void ReplaceCommand::Undo() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	Editor editor(this->parent);
	editor.Replace(this->offset, this->replaced, this->source, TRUE, this->columnIndex);
}

void ReplaceCommand::Redo() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	Editor editor(this->parent);
	editor.Replace(this->offset, this->source, this->replaced, TRUE, this->columnIndex);
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}
