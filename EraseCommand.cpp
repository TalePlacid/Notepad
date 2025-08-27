#include <afxwin.h>
#include "EraseCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "GlyphFactory.h"

#pragma warning(disable:4996)

EraseCommand::EraseCommand(CWnd* parent)
	:UndoableCommand(parent) {

}

EraseCommand::~EraseCommand() {

}

EraseCommand::EraseCommand(const EraseCommand& source)
	:UndoableCommand(source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
}

EraseCommand& EraseCommand::operator=(const EraseCommand& source) {
	UndoableCommand::operator=(source);

	this->character[0] = source.character[0];
	this->character[1] = source.character[1];

	return *this;
}

void EraseCommand::Execute() {
	if (!((NotepadForm*)(this->parent))->IsCompositing())
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long columnIndex = row->GetCurrent();
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
	}
}

void EraseCommand::Undo() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	if (character[0] != '\r')
	{
		row->Add(columnIndex, glyph);
	}
	else
	{
		note->Add(rowIndex + 1, glyph);
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Add(character);
}

Command* EraseCommand::Clone() {
	return new EraseCommand(*this);
}