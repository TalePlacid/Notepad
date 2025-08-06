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

	//2. 선택범위를 지운다.
	Long i;
	if (startRowIndex < endRowIndex)
	{
		row = note->GetAt(startRowIndex);
		row->TruncateAfter(startColumnIndex);

		while (endRowIndex - 1 > startRowIndex)
		{
			note->Remove(startRowIndex + 1);
			endRowIndex--;
		}

		row = note->GetAt(endRowIndex);
		row->TruncateBefore(endColumnIndex);

		note->MergeRows(startRowIndex);
	}
	else
	{
		row = note->GetAt(startRowIndex);
		while (endColumnIndex - 1 >= startColumnIndex)
		{
			row->Remove(startColumnIndex);
			endColumnIndex--;
		}
	}

	note->Move(startRowIndex);
	row = note->GetAt(startRowIndex);
	row->Move(startColumnIndex);

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