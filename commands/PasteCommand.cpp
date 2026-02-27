#include "PasteCommand.h"
#include "../Editor.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../ClipboardController.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../ByteChecker.h"
#include "../RowCounter.h"
#include "../CaretNavigator.h"
#include "../NoteWrapper.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

PasteCommand::PasteCommand(CWnd* parent)
	:Command(parent), erased(""), contents("") {
	this->frontOffset = -1;
	this->rearOffset = -1;
	this->columnIndex = 0;
}

PasteCommand::~PasteCommand() {

}

PasteCommand::PasteCommand(const PasteCommand& source)
	:Command(source), contents(source.contents) {
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
	this->erased = source.erased;
	this->contents = source.contents;
}

PasteCommand& PasteCommand::operator=(const PasteCommand& source) {
	Command::operator=(source);

	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
	this->erased = source.erased;
	this->contents = source.contents;

	return *this;
}

void PasteCommand::Execute() {
	//1. 클립보드에서 붙여넣기가 됐으면,
	ClipboardController* clipboardController = ((NotepadForm*)(this->parent))->clipboardController;
	BOOL isPasted = clipboardController->Paste();
	if (isPasted)
	{
		//2. 선택범위가 있으면, 지운다.
		this->isUndoable = true;

		Editor editor(this->parent);
		BOOL isSelected = editor.GetSelectedRange(frontOffset, rearOffset);
		if (isSelected)
		{
			editor.EraseRange(this->frontOffset, this->rearOffset, this->columnIndex, this->erased);
		}

		//3. 내용을 붙여넣는다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->frontOffset = pagingBuffer->GetCurrentOffset();
		this->contents = clipboardController->GetContent();
		editor.InsertTextAt(this->frontOffset, this->columnIndex, this->contents);
	}
}

void PasteCommand::Undo() {
	//1. 붙여넣은 내용을 지운다.
	Editor editor(this->parent);
	Long rearOffset = this->frontOffset + this->contents.GetLength();
	CString erased;
	editor.EraseRange(this->frontOffset, rearOffset, this->columnIndex, erased);
	
	//2. 삭제한 내용이 있으면, 삽입한다.
	if (this->erased != "")
	{
		editor.InsertTextAt(this->frontOffset, this->columnIndex, this->erased, TRUE);
	}
}

void PasteCommand::Redo() {
	Editor editor(this->parent);
	if (this->erased != "")
	{
		editor.EraseRange(this->frontOffset, this->rearOffset, this->columnIndex, this->erased);
	}

	editor.InsertTextAt(this->frontOffset, this->columnIndex, this->contents, TRUE);
}

Command* PasteCommand::Clone() {
	return new PasteCommand(*this);
}