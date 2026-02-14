#include <afxwin.h>
#include "ShiftDownAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"

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

	//2. 적재 범위에서 벗어나면 적재한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * sizeCalculator->GetRowHeight();
	if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
	}

	//3. 마지막 줄이 아니라면,
	if (rowIndex + 1 < note->GetLength())
	{
		//3.1. 현재 위치까지의 너비를 구한다.
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
		Long originalWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//3.2. 줄의 끝까지 반복한다.
		while (columnIndex < row->GetLength())
		{
			row->GetAt(columnIndex)->ToggleSelection();
			columnIndex = row->Next();

			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//3.3. 노트에서 이동한다.
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		if (!row->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->NextRow();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//3.4. 가장 비슷한 위치까지 반복한다.
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(row, originalWidth);
		while (columnIndex < nearestIndex)
		{
			row->GetAt(columnIndex)->ToggleSelection();
			columnIndex = row->Next();

			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}