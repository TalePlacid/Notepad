#include <afxwin.h>
#include "EraseRangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

EraseRangeCommand::EraseRangeCommand(CWnd* parent)
	:Command(parent) {

}

EraseRangeCommand::~EraseRangeCommand() {

}

void EraseRangeCommand::Execute() {
	//1. 노트에서 선택된 범위를 찾는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long startRowIndex = note->FindSelectionStart();
	Glyph* row = note->GetAt(startRowIndex);
	Long startColumnIndex = row->FindSelectionStart();

	Long endRowIndex = note->FindSelectionEnd();
	row = note->GetAt(endRowIndex);
	Long endColumnIndex = row->FindSelectionEnd();

#if 0
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Long i;
	Long j;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	if (pagingBuffer->GetCurrentOffset() > selectionBeginOffset)
	{
		i = columnIndex - 1;
		while (i >= 0 && pagingBuffer->GetCurrentOffset() > selectionBeginOffset)
		{
			row->Remove(i);
			pagingBuffer->Remove();
			i--;
		}

		Glyph* previousRow;
		Long previousRowLength;
		Long j;
		i = rowIndex - 1;
		while (i > 0 && pagingBuffer->GetCurrentOffset() > selectionBeginOffset)
		{
			previousRow = note->GetAt(i);
			previousRowLength = previousRow->GetLength();
			j = 0;
			while (j < row->GetLength())
			{
				previousRow->Add(row->GetAt(j)->Clone());
				j++;
			}

			note->Remove(i + 1);
			pagingBuffer->Remove();
			
			j = previousRowLength - 1;
			while (j >= 0 && pagingBuffer->GetCurrentOffset() > selectionBeginOffset)
			{
				previousRow->Remove(j);
				pagingBuffer->Remove();
				j--;
			}

			row = previousRow;
			i--;
		}

		while (pagingBuffer->GetCurrentOffset() > selectionBeginOffset)
		{
			pagingBuffer->Remove();
		}

		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
		}
	}
	else if (pagingBuffer->GetCurrentOffset() < selectionBeginOffset)
	{
		while (row->GetLength() > columnIndex && pagingBuffer->GetCurrentOffset() < selectionBeginOffset)
		{
			row->Remove(columnIndex);
			pagingBuffer->Next();
			pagingBuffer->Remove();
		}

		while (note->GetLength() - 1 > rowIndex && pagingBuffer->GetCurrentOffset() < selectionBeginOffset)
		{
			Glyph* nextRow = note->GetAt(rowIndex + 1);
			Long rowLength = row->GetLength();
			i = 0;
			while (i < nextRow->GetLength())
			{
				row->Add(nextRow->GetAt(i)->Clone());
				i++;
			}

			note->Remove(rowIndex + 1);
			pagingBuffer->NextRow();
			pagingBuffer->Remove();

			while (row->GetLength() > columnIndex && pagingBuffer->GetCurrentOffset() < selectionBeginOffset)
			{
				row->Remove(columnIndex);
				pagingBuffer->Next();
				pagingBuffer->Remove();
			}
		}

		while (pagingBuffer->GetCurrentOffset() < selectionBeginOffset)
		{
			pagingBuffer->Next();
			pagingBuffer->Remove();
		}

		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}
	}

#endif
}