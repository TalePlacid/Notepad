#include <afxwin.h>
#include "LoadPreviousCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../SuspendAutoWrap.h"

#pragma warning(disable:4996)

LoadPreviousCommand::LoadPreviousCommand(CWnd* parent)
	:Command(parent) {

}

LoadPreviousCommand::~LoadPreviousCommand() {

}

void LoadPreviousCommand::Execute() {
	//자동개행시 임시 개행 풀기
	SuspendAutoWrap suspendAutoWrap(this->parent);

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
	Long count = note->AppendFromFront(loadedNote);
	pagingBuffer->CacheRowStartIndex(-count);
	
	//5. 선택여부를 반영한다.
	Long previousRowIndex = -1;
	rowIndex += count;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long i = pagingBuffer->GetCurrentOffset();
	while (rowIndex >= 0 && previousRowIndex != rowIndex && selectionBeginOffset >= 0 && i > selectionBeginOffset)
	{
		while (columnIndex > 0 && i > selectionBeginOffset)
		{
			row->GetAt(columnIndex - 1)->Select(true);
			columnIndex = row->Previous();
			i = pagingBuffer->Previous();
		}

		previousRowIndex = rowIndex;
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();
		i = pagingBuffer->PreviousRow();
		i = pagingBuffer->Last();
	}

	//6. 현재 위치로 돌아온다.
	currentRowIndex = note->Move(currentRowIndex + count);
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->Move(currentColumnIndex);
	pagingBuffer->MoveOffset(currentOffset);

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	//7. 노트에서 아랫 부분을 지운다.
	Long belowIndex = currentRowIndex + PAGE_ROWCOUNT;
	note->TruncateAfter(belowIndex);

	//8. 수평 스크롤바 최대값을 갱신한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long rowWidth = 0;
	Long max = scrollController->GetHScroll().GetMax();
	i = 0;
	while (i < note->GetLength())
	{
		rowWidth = sizeCalculator->GetRowWidth(note->GetAt(i)->MakeString().c_str());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		i++;
	}
	scrollController->ResizeHRange(max);
}