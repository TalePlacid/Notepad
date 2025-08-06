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
	Long markedRowIndex = note->GetMarked();
	Glyph* row = note->GetAt(markedRowIndex);
	Long markedColumnIndex = row->GetMarked();
	Position markedPosition(markedRowIndex, markedColumnIndex);

	Long currentRowIndex = note->GetCurrent();
	row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	Position currentPosition(currentRowIndex, currentColumnIndex);

	Position startPosition;
	Position endPosition;
	if (currentPosition < markedPosition)
	{
		startPosition = currentPosition;
		endPosition = markedPosition;
	}
	else if (currentPosition > markedPosition)
	{
		startPosition = markedPosition;
		endPosition = currentPosition;
	}

	//2. 선택범위를 지운다.
	Long i;
	if (startPosition.GetRow() < endPosition.GetRow())
	{
		row = note->GetAt(startPosition.GetRow());
		row->TruncateAfter(startPosition.GetColumn());

		while (endPosition.GetRow() - 1 > startPosition.GetRow())
		{
			note->Remove(startPosition.GetRow() + 1);
			endPosition = endPosition.Up();
		}

		row = note->GetAt(endPosition.GetRow());
		row->TruncateBefore(endPosition.GetColumn());

		note->MergeRows(startPosition.GetRow());
	}
	else
	{
		row = note->GetAt(startPosition.GetRow());
		while (endPosition.GetColumn() - 1 >= startPosition.GetColumn())
		{
			row->Remove(startPosition.GetColumn());
			endPosition = endPosition.Left();
		}
	}

	note->Move(startPosition.GetRow());
	row = note->GetAt(startPosition.GetRow());
	row->Move(startPosition.GetColumn());

	//3. 페이징버퍼에서 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	pagingBuffer->Remove(pagingBuffer->GetSelectionBeginOffset());
	pagingBuffer->UnmarkSelectionBegin();

	if (currentOffset > selectionBeginOffset)
	{
		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
		}
	}
	else if (currentOffset < selectionBeginOffset)
	{
		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}
	}

	//4. 스크롤바 위치를 조정한다.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

	//5. 클라이언트 영역을 갱신한다.
	this->parent->Invalidate();
}