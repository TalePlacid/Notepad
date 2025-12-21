#include <afxwin.h>
#include "LoadLastCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

LoadLastCommand::LoadLastCommand(CWnd* parent)
	:Command(parent) {

}

LoadLastCommand::~LoadLastCommand() {

}

void LoadLastCommand::Execute() {
	//1. 페이징버퍼에서 마지막위치로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->LastRow();
	pagingBuffer->Last();

	//2. 페이징버퍼에서 한 줄을 추가한다.
	TCHAR character[2];
	character[0] = '\r';
	character[1] = '\n';
	Long last = pagingBuffer->Add(character);

	//3. 페이징 버퍼에서 이전 부분을 적재한다.
	Glyph* loadedNote = pagingBuffer->LoadPrevious();

	//4. 페이지버퍼에서 추가했던 줄을 지운다.
	pagingBuffer->MoveOffset(last);
	pagingBuffer->Remove();

	//5. 페이징버퍼에서 캐싱한다.
	Long fileEndOffset = pagingBuffer->GetFileEndOffset();
	Long totalRowCount = pagingBuffer->CountRow(fileEndOffset);
	Long difference = totalRowCount - loadedNote->GetLength() - pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(difference);

	//6. 노트를 교체한다.
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	((NotepadForm*)(this->parent))->note = loadedNote;

	//7. 노트에서 마지막으로 이동한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Last();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Last();

	//8. 선택여부를 반영한다.
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long offset = pagingBuffer->GetCurrentOffset();
	Long previousColumnIndex;
	Long previousRowIndex = -1;
	while (rowIndex != previousRowIndex && offset >= selectionBeginOffset)
	{
		previousColumnIndex = columnIndex;
		columnIndex = row->Previous();
		offset = pagingBuffer->Previous();
		while (columnIndex != previousColumnIndex && offset >= selectionBeginOffset)
		{
			row->GetAt(columnIndex)->Select(true);
			previousColumnIndex = columnIndex;
			columnIndex = row->Previous();
			offset = pagingBuffer->Previous();
		}

		previousRowIndex = rowIndex;
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();
		offset = pagingBuffer->PreviousRow();
		offset = pagingBuffer->Last();
	}

	//9. 노트에서 마지막으로 이동한다.
	rowIndex = note->Last();
	row = note->GetAt(rowIndex);
	columnIndex = row->Last();
	pagingBuffer->MoveOffset(pagingBuffer->GetFileEndOffset());
}