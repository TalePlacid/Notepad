#include <afxwin.h>
#include "EraseRangeCommand.h"
#include "../NotepadForm.h"
#include "../Editor.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../ByteChecker.h"
#include "../CaretNavigator.h"
#include "../RowCounter.h"
#include "../NoteWrapper.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

EraseRangeCommand::EraseRangeCommand(CWnd* parent)
	:Command(parent), contents("") {
	this->frontOffset = -1;
	this->rearOffset = -1;
	this->columnIndex = 0;
}

EraseRangeCommand::~EraseRangeCommand() {

}

EraseRangeCommand::EraseRangeCommand(const EraseRangeCommand& source)
	:Command(source), contents(source.contents) {
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
}

EraseRangeCommand& EraseRangeCommand::operator=(const EraseRangeCommand& source) {
	Command::operator=(source);

	this->contents = source.contents;
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;

	return *this;
}

void EraseRangeCommand::Execute() {
	//1. 선택 범위를 읽는다.
	Editor editor(this->parent);
	this->isUndoable = editor.GetSelectedRange(this->frontOffset, this->rearOffset);

	//2. 선택범위가 있으면, 지운다.
	if (this->isUndoable)
	{
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->contents = pagingBuffer->MakeSelectedString();
		editor.EraseRange(this->frontOffset, this->rearOffset, this->columnIndex);
	}
}

void EraseRangeCommand::Undo() {
	Editor editor(this->parent);
	editor.InsertTextAt(this->frontOffset, this->columnIndex, this->contents, true);
}   

void EraseRangeCommand::Redo() {
	Editor editor(this->parent);
	editor.EraseRange(this->frontOffset, this->rearOffset, this->columnIndex);
}

Command* EraseRangeCommand::Clone() {
	return new EraseRangeCommand(*this);
}
