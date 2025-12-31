#include <afxwin.h>
#include "ShiftUpAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftUpAction::ShiftUpAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftUpAction::~ShiftUpAction() {

}

void ShiftUpAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 이전 줄이 적재범위에서 벗어났다면 재적재한다.
	if (note->IsAboveTopLine(rowIndex - 1))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
		rowIndex = note->GetCurrent();
	}

	//3. 현재 위치가 첫번째 줄이 아니라면,
	if (rowIndex > 0)
	{
		//3.1. 현재 위치까지의 너비를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//3.2. 현재 줄의 앞부분을 처리한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long selectionBeginOffset;
		Long previousOffset = -1;
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		while (previousOffset != currentOffset)
		{
			//3.2.1. 페이징 버퍼에서 마킹한다.
			selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
			if (selectionBeginOffset < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			else if (currentOffset == selectionBeginOffset)
			{
				pagingBuffer->UnmarkSelectionBegin();
			}

			//3.2.2. 노트에서 선택반전한다.
			if (columnIndex > 0)
			{
				row->GetAt(columnIndex - 1)->ToggleSelection();
			}

			previousOffset = currentOffset;
			currentOffset = pagingBuffer->Previous();
			if (currentOffset == selectionBeginOffset)
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
			columnIndex = row->Previous();
		}

		//3.3. 이전 줄로 이동한다.
		currentOffset = pagingBuffer->PreviousRow();
		currentOffset = pagingBuffer->Last();
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();

		//3.4. 너비에 근접한 열부터 뒷부분을 선택한다.
		Long nearestColumnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
		Long i = row->GetLength();
		while (previousOffset != currentOffset && i > nearestColumnIndex)
		{
			//3.4.1. 페이징 버퍼에서 마킹한다.
			selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
			if (selectionBeginOffset < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			else if (currentOffset == selectionBeginOffset)
			{
				pagingBuffer->UnmarkSelectionBegin();
			}

			//3.4.2. 노트에서 선택반전한다.
			if (columnIndex > 0)
			{
				row->GetAt(columnIndex - 1)->ToggleSelection();
			}

			previousOffset = currentOffset;
			currentOffset = pagingBuffer->Previous();
			if (currentOffset == selectionBeginOffset)
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
			columnIndex = row->Previous();
			i--;
		}
	}
}