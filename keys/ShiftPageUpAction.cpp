#include <afxwin.h>
#include "ShiftPageUpAction.h"
#include "../resource.h"
#include "../ScrollController.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

ShiftPageUpAction::ShiftPageUpAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftPageUpAction::~ShiftPageUpAction() {

}

void ShiftPageUpAction::Perform() {
	//1. 수직스크롤바가 있고, 처음 위치가 아니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	if (scrollController->HasVScroll() && vScroll.GetPos() > 0)
	{
		//1.1. 올라갈 줄 수를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long rowCount = vScroll.GetPage() / rowHeight;
		if (vScroll.GetPage() % rowHeight > 0)
		{
			rowCount++;
		}

		//1.2. 노트에서 현재 줄의 너비를 구한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Long originalRowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//1.3. 첫번째 줄이 아니고, 올라갈 줄 수가 남았으면,
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long i = 0;
		while (rowIndex > 0 && i < rowCount)
		{
			//1.3.1. 적재범위를 넘어서면, 적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
				rowIndex = note->GetCurrent();
			}
			
			//3.2. 줄의 처음까지 반복한다.
			row = note->GetAt(rowIndex);
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

			i++;
		}
	}
}