#include <afxwin.h>
#include "LoadPreviousCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

LoadPreviousCommand::LoadPreviousCommand(CWnd* parent)
	:Command(parent) {

}

LoadPreviousCommand::~LoadPreviousCommand() {

}

void LoadPreviousCommand::Execute() {
	//1. 노트에서 현재 위치를 기억한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	//2. 노트의 시작점으로 이동한다.
	Long rowIndex = note->First();
	pagingBuffer->PreviousRow(currentRowIndex - rowIndex);
	row = note->GetAt(rowIndex);
	Long columnIndex = row->First();
	pagingBuffer->First();

	//3. 앞 부분을 로드한다.
	Glyph* loadedNote = pagingBuffer->LoadPrevious();

	//4. 로드된 분량을 붙인다.
	note->AppendFromFront(loadedNote);
	pagingBuffer->CacheRowStartIndex(-loadedNote->GetLength());

	//5. 현재 위치로 돌아온다.
	currentRowIndex = note->Move(currentRowIndex + loadedNote->GetLength());
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->Move(currentColumnIndex);
	pagingBuffer->MoveOffset(currentOffset);

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	//6. 노트에서 아랫 부분을 지운다.
	Long belowIndex = currentRowIndex + PAGE_ROWCOUNT;
	note->TruncateAfter(belowIndex);
}