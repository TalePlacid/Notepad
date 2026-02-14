#include <afxwin.h>
#include "ShiftRightAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

ShiftRightAction::ShiftRightAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftRightAction::~ShiftRightAction() {

}

void ShiftRightAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 끝이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		//2.1. 노트에서 선택한다.
		row->GetAt(columnIndex)->ToggleSelection();

		//2.2. 노트에서 이동한다.
		columnIndex = row->Next();

		//2.3. 페이징 버퍼에서 선택과 이동한다.
		pagingBuffer->BeginSelectionIfNeeded();
		pagingBuffer->Next();
		pagingBuffer->EndSelectionIfCollapsed();
	}
	else //3. 줄의 끝이면,
	{
		//3.1. 적재범위를 벗어나면, 재적재한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * sizeCalculator->GetRowHeight();
		if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}

		//3.2. 마지막줄이 아니면,
		if (rowIndex + 1 < note->GetLength())
		{
			//3.2.1. 노트에서 이동한다.
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			//3.2.2. 줄이 진짜이면, 페이징 버퍼에서 선택과 이동한다.
			if (!row->IsDummyRow())
			{
				pagingBuffer->BeginSelectionIfNeeded();
				pagingBuffer->NextRow();
				pagingBuffer->EndSelectionIfCollapsed();
			}
		}
	}
}