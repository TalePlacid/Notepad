#include "SelectToPointAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../CoordinateConverter.h"

#pragma warning(disable:4996)

SelectToPointAction::SelectToPointAction(CWnd* parent, CPoint point)
	:Action(parent), point(point) {

}

SelectToPointAction::~SelectToPointAction() {

}

void SelectToPointAction::Perform() {
	//1. 클릭된 위치를 구한다.
	CoordinateConverter coordinateConverter(this->parent);
	Long clickedRowIndex;
	Long clickedColumnIndex;
	coordinateConverter.AbsoluteToNotePosition(this->point, clickedRowIndex, clickedColumnIndex);

	//2. 현재위치부터 클릭된 위치까지 선택한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	// 클릭된 위치가 더 이전이면,
	Glyph* character;
	Long bytes = 0;
	if (clickedRowIndex < rowIndex || (clickedRowIndex == rowIndex && clickedColumnIndex < columnIndex))
	{
		BOOL isRealRow;
		while (rowIndex > clickedRowIndex)
		{
			//칸이동
			while (columnIndex > 0)
			{
				pagingBuffer->BeginSelectionIfNeeded();
				columnIndex = row->Previous();
				character = row->GetAt(columnIndex);
				character->ToggleSelection();
				bytes = character->GetBytes();
				currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
				pagingBuffer->EndSelectionIfCollapsed();
			}

			//줄이동
			isRealRow = !row->IsDummyRow();
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->Last();
			if (isRealRow)
			{
				pagingBuffer->BeginSelectionIfNeeded();
				currentOffset = pagingBuffer->MoveOffset(currentOffset - 2);
				pagingBuffer->EndSelectionIfCollapsed();
			}
		}

		while (columnIndex > clickedColumnIndex)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			columnIndex = row->Previous();
			character = row->GetAt(columnIndex);
			character->ToggleSelection();
			bytes = character->GetBytes();
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
	else // 클릭된 위치가 더 이후이면,
	{
		while (rowIndex < clickedRowIndex)
		{
			//칸 이동
			while (columnIndex < row->GetLength())
			{
				pagingBuffer->BeginSelectionIfNeeded();
				character = row->GetAt(columnIndex);
				character->ToggleSelection();
				bytes = character->GetBytes();
				columnIndex = row->Next();
				currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
				pagingBuffer->EndSelectionIfCollapsed();
			}

			//줄 이동
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			if (!row->IsDummyRow())
			{
				pagingBuffer->BeginSelectionIfNeeded();
				currentOffset = pagingBuffer->MoveOffset(currentOffset + 2);
				pagingBuffer->EndSelectionIfCollapsed();
			}
		}

		while (columnIndex < clickedColumnIndex)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			character = row->GetAt(columnIndex);
			character->ToggleSelection();
			bytes = character->GetBytes();
			columnIndex = row->Next();
			currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}