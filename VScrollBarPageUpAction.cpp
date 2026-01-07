#include <afxwin.h>
#include "VScrollBarPageUpAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarPageUpAction::VScrollBarPageUpAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageUpAction::~VScrollBarPageUpAction() {

}

void VScrollBarPageUpAction::Perform() {
	//1. 스크롤을 한 줄 올린다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	Long pos = vScroll.GetPos() - page;
	if (pos < 0)
	{
		pos = 0;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 보이는 영역을 벗어났다면,
	if (currentPos + rowHeight > pos + vScroll.GetPage())
	{
		//3.1. 현재 줄 너비를 구한다.
		Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//3.2. 화면 끝에 해당하는 줄 위치를 찾는다.
		Long rowIndexToMove = (pos + page) / rowHeight - rowStartIndex - 1;
		if (rowIndexToMove < 0)
		{
			rowIndexToMove = 0;
		}

		//3.3. 적재범위를 넘어섰으면, 재적재한다.
		if (note->IsAboveTopLine(rowIndexToMove) && rowStartIndex > 0)
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			rowStartIndex = pagingBuffer->GetRowStartIndex();
			rowIndexToMove = (pos + page) / rowHeight - rowStartIndex - 1;
			if (rowIndexToMove < 0)
			{
				rowIndexToMove = 0;
			}
		}

		//3.4. 줄 수 만큼 반복한다.
		Long nearestIndex;
		Glyph* previousRow;
		Long rowCount = rowIndex - rowIndexToMove;
		Long i = 0;
		while (i < rowCount && rowIndex > 0)
		{
			//3.4.1. 줄의 처음까지 반복한다.
			row = note->GetAt(rowIndex);
			while (columnIndex > 0)
			{
				columnIndex = row->Previous();
				pagingBuffer->Previous();
			}

			//3.4.2. 윗 줄로 이동한다.
			previousRow = row;
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->Last();

			if (!previousRow->IsDummyRow())
			{
				pagingBuffer->PreviousRow();
				pagingBuffer->Last();
			}

			//3.4.3. 가까운 위치까지 반복한다.
			nearestIndex = sizeCalculator->GetNearestColumnIndex(row, columnIndex);
			while (columnIndex > nearestIndex)
			{
				columnIndex = row->Previous();
				pagingBuffer->Previous();
			}

			i++;
		}
	}
}