#include <afxwin.h>
#include "ShiftDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftDownAction::ShiftDownAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftDownAction::~ShiftDownAction() {

}

void ShiftDownAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 다음 줄이 적재범위에서 벗어났다면 재적재한다.
	if (note->IsBelowBottomLine(rowIndex + 1))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
	}

	//3. 현재 위치가 마지막 줄이 아니라면,
	if (rowIndex < note->GetLength() - 1)
	{
		//3.1. 현재 위치까지의 너비를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//3.2. 현재 줄의 뒷부분을 선택한다.
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
			if (columnIndex < row->GetLength())
			{
				row->GetAt(columnIndex)->ToggleSelection();
			}

			previousOffset = currentOffset;
			currentOffset = pagingBuffer->Next();
			if (currentOffset == selectionBeginOffset)
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
			columnIndex = row->Next();
		}

		//3.3. 다음줄로 이동한다.
		previousOffset = currentOffset;
		currentOffset = pagingBuffer->NextRow();
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		//3.4. 너비에 근접한 열까지 반복한다.
		Long nearestColumnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
		Long i = 0;
		while (previousOffset != currentOffset && i < nearestColumnIndex)
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
			if (columnIndex < row->GetLength())
			{
				row->GetAt(columnIndex)->ToggleSelection();
			}

			previousOffset = currentOffset;
			currentOffset = pagingBuffer->Next();
			if (currentOffset == selectionBeginOffset)
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
			columnIndex = row->Next();
			i++;
		}
	}
}