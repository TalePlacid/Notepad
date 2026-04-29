#include <afxwin.h>
#include "SelectDownAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

SelectDownAction::SelectDownAction(CWnd* parent)
	:Action(parent) {

}

SelectDownAction::~SelectDownAction() {

}

void SelectDownAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 적재 범위에서 벗어나면 적재한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (!note->IsLastPage() && note->IsBelowBottomLine(rowIndex + 1))
	{
		PageManager::LoadNext(this->parent);
		rowIndex = note->GetCurrent();
	}

	//3. 마지막 줄이 아니라면,
	if (rowIndex + 1 < note->GetLength())
	{
		//3.1. 현재 위치까지의 너비를 구한다.
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
		Long originalWidth = sizeCalculator->GetRowWidth(rowIndex, columnIndex);

		//3.2. 줄의 끝까지 반복한다.
		Long bytes;
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		while (columnIndex < row->GetLength())
		{
			row->GetAt(columnIndex)->ToggleSelection();
			bytes = row->GetAt(columnIndex)->GetBytes();
			columnIndex = row->Next();

			pagingBuffer->BeginSelectionIfNeeded();
			currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//3.3. 노트에서 이동한다.
		bytes = row->GetNextBytes(columnIndex) + 2;
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		if (!row->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//3.4. 가장 비슷한 위치까지 반복한다.
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, originalWidth);
		while (columnIndex < nearestIndex)
		{
			row->GetAt(columnIndex)->ToggleSelection();
			bytes = row->GetAt(columnIndex)->GetBytes();
			columnIndex = row->Next();

			pagingBuffer->BeginSelectionIfNeeded();
			currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}
