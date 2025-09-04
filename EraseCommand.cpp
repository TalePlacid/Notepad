#include "EraseCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "GlyphFactory.h"
#include "resource.h"

#pragma warning(disable:4996)

EraseCommand::EraseCommand(CWnd* parent)
	:UndoableCommand(parent) {
	this->offset = -1;
}

EraseCommand::~EraseCommand() {

}

EraseCommand::EraseCommand(const EraseCommand& source)
	:UndoableCommand(source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->offset = source.offset;
}

EraseCommand& EraseCommand::operator=(const EraseCommand& source) {
	UndoableCommand::operator=(source);

	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->offset = source.offset;

	return *this;
}

void EraseCommand::Execute() {
	Glyph* note;
	Glyph* row;
	Long rowIndex;
	Long columnIndex;
	
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (this->offset >= 0)
	{
		pagingBuffer->MoveOffset(this->offset);
		if (!pagingBuffer->IsOnPage(this->offset))
		{
			pagingBuffer->Load();
		}
		note = ((NotepadForm*)(this->parent))->note;
		rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
		row = note->GetAt(rowIndex);
		row->Move(pagingBuffer->GetCurrent().GetColumn());
	}

	note = ((NotepadForm*)(this->parent))->note;
	rowIndex = note->GetCurrent();
	row = note->GetAt(rowIndex);

	columnIndex = row->GetCurrent();
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

			if (!pagingBuffer->IsBelowTopLine())
			{
				pagingBuffer->Load();
			}
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();	
}

void EraseCommand::Undo() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MoveOffset(this->offset);
	if (!pagingBuffer->IsOnPage(this->offset))
	{
		pagingBuffer->Load();
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Move(pagingBuffer->GetCurrent().GetColumn());

	if (character[0] != '\r')
	{
		row->Add(columnIndex, glyph);
	}
	else
	{
		note->Add(rowIndex + 1, glyph);
	}

	pagingBuffer->Add(character);

	this->offset = pagingBuffer->GetCurrentOffset();
}

Command* EraseCommand::Clone() {
	return new EraseCommand(*this);
}

UINT EraseCommand::GetId() {
	return ID_COMMAND_ERASE;
}