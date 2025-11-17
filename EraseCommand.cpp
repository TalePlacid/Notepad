#include "EraseCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "GlyphFactory.h"
#include "resource.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingNavigator.h"

#pragma warning(disable:4996)

EraseCommand::EraseCommand(CWnd* parent)
	:Command(parent) {
	this->offset = -1;
}

EraseCommand::~EraseCommand() {

}

EraseCommand::EraseCommand(const EraseCommand& source)
	:Command(source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->offset = source.offset;
}

EraseCommand& EraseCommand::operator=(const EraseCommand& source) {
	Command::operator=(source);

	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->offset = source.offset;

	return *this;
}

void EraseCommand::Execute() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		Glyph* letter = row->GetAt(columnIndex - 1);
		this->character[0] = letter->MakeString()[0];
		if (letter->IsMultiByteCharacter())
		{
			this->character[1] = letter->MakeString()[1];
		}

		row->Remove(columnIndex - 1);
		pagingBuffer->Remove();
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex > 0)
		{
			this->character[0] = '\r';
			this->character[1] = '\n';

			Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex - 1);
			Long previousCurrent = previousRow->Last();
			rowIndex = note->MergeRows(rowIndex - 1);
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			row->Move(previousCurrent);

			pagingBuffer->Remove();
			previousRow->Move(previousCurrent);
			
			ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
			SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
			Long max = scrollController->GetVScroll().GetMax() - sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();	
}

void EraseCommand::Undo() {
	GlyphFactory glyphFactory;
	Glyph* glyph;

	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (this->character[0] != '\r')
	{
		glyph = glyphFactory.Create(this->character);
		row->Add(columnIndex, glyph);
	}
	else
	{
		if (columnIndex < row->GetLength() - 1)
		{
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}
		else
		{
			glyph = glyphFactory.Create(this->character);
			rowIndex = note->Add(rowIndex + 1, glyph);
		}

		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}
	pagingBuffer->Add(character);

	this->offset = pagingBuffer->GetCurrentOffset();
}

void EraseCommand::Redo() {
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		row->Remove(columnIndex - 1);
		pagingBuffer->Remove();
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex > 0)
		{
			Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex - 1);
			Long previousCurrent = previousRow->Last();
			rowIndex = note->MergeRows(rowIndex - 1);
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			row->Move(previousCurrent);

			pagingBuffer->Remove();
			previousRow->Move(previousCurrent);
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();
}

Command* EraseCommand::Clone() {
	return new EraseCommand(*this);
}

UINT EraseCommand::GetId() {
	return ID_COMMAND_ERASE;
}

bool EraseCommand::IsUndoable() {
	return true;
}