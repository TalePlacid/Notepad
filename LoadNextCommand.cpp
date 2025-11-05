#include <afxwin.h>
#include "LoadNextCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

LoadNextCommand::LoadNextCommand(CWnd* parent)
	:Command(parent) {

}

LoadNextCommand::~LoadNextCommand() {

}

void LoadNextCommand::Execute() {
	//1. 노트에서 현재 위치를 기억한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	//2. 노트의 마지막 위치로 이동한다.
	Long rowIndex = note->Move(note->GetLength() - 1);
	pagingBuffer->NextRow(rowIndex - currentRowIndex);
	row = note->GetAt(rowIndex);
	Long columnIndex = row->Last();
	pagingBuffer->Last();

	//3. 뒷 부분을 로드한다.
	if (currentOffset > 0)
	{
		pagingBuffer->NextRow();
		pagingBuffer->First();
	}
	Glyph* loadedNote = pagingBuffer->LoadNext();

	//4. 로드된 뒷 부분을 기존의 분량에 합친다.
	note->AppendFromRear(loadedNote);
	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	//5. 현재 위치로 돌아온다.
	currentRowIndex = note->Move(currentRowIndex);
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->Move(currentColumnIndex);
	pagingBuffer->MoveOffset(currentOffset);

	//6. 노트에서 윗 부분을 지운다.
	Long upperIndex = currentRowIndex - PAGE_ROWCOUNT;
	if (upperIndex < 0)
	{
		upperIndex = 0;
	}
	note->TruncateBefore(upperIndex);
	pagingBuffer->CacheRowStartIndex(upperIndex);
}