#include <afxwin.h>
#include "ShiftUpAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../PageLoader.h"

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

	//2. 적재범위를 넘어서면, 적재한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
	{
		PageLoader::LoadPrevious(this->parent);
		rowIndex = note->GetCurrent();
	}

	//3. 첫번째 줄이 아니면,
	if (rowIndex > 0)
	{
		//3.1. 현재 위치까지의 너비를 구한다.
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long originalRowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//3.2. 줄의 처음까지 반복한다.
		while (columnIndex > 0)
		{
			columnIndex = row->Previous();
			row->GetAt(columnIndex)->ToggleSelection();

			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//3.3. 이전 줄로 이동한다.
		rowIndex = note->Previous();
		Glyph* movedRow = note->GetAt(rowIndex);
		columnIndex = movedRow->Last();

		if (!row->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//3.4. 가장 가까운 위치까지 반복한다.
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(movedRow, originalRowWidth);
		while (columnIndex > nearestIndex && columnIndex > 0)
		{
			columnIndex = movedRow->Previous();
			movedRow->GetAt(columnIndex)->ToggleSelection();

			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}