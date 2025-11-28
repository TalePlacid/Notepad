#include <afxwin.h>
#include "LoadFirstCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"

#pragma warning(disable:4996)

LoadFirstCommand::LoadFirstCommand(CWnd* parent)
	:Command(parent) {

}

LoadFirstCommand::~LoadFirstCommand() {

}

void LoadFirstCommand::Execute() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->FirstRow();

	Glyph* loadedNote = pagingBuffer->LoadNext();

	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(-rowStartIndex);

	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	((NotepadForm*)(this->parent))->note = loadedNote;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->First();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->First();

	Long i = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long previousRowIndex = -1;
	while (rowIndex != previousRowIndex && i < selectionBeginOffset)
	{
		while (columnIndex < row->GetLength() && i < selectionBeginOffset)
		{
			row->GetAt(columnIndex)->Select(true);
			columnIndex = row->Next();
			i = pagingBuffer->Next();
		}

		previousRowIndex = rowIndex;
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		i = pagingBuffer->NextRow();
	}

	rowIndex = note->First();
	row = note->GetAt(rowIndex);
	columnIndex = row->First();
	pagingBuffer->FirstRow();
}