#include <afxwin.h>
#include "VScrollBarPageDownAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarPageDownAction::VScrollBarPageDownAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageDownAction::~VScrollBarPageDownAction() {

}

void VScrollBarPageDownAction::Perform() {
	//1. 스크롤 크기만큼 페이지를 내린다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() + vScroll.GetPage();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 현재위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 현재 줄의 위치가 보이는 영역을 벗어났으면,
	if (currentPos < pos)
	{
		//3.1. 현재 줄 너비를 구한다.
		Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//3.2. 스크롤에 해당하는 줄 위치를 구한다.
		Long rowIndexToMove = pos / rowHeight - rowStartIndex;
		if (pos % rowHeight > 0)
		{
			rowIndexToMove++;
		}

		//3.3. 적재범위를 벗어나면, 재적재한다.
		Long pageMax = (rowStartIndex + note->GetLength()) * rowHeight;
		if (note->IsBelowBottomLine(rowIndexToMove) && pageMax < vScroll.GetMax())
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			rowStartIndex = pagingBuffer->GetRowStartIndex();
			rowIndexToMove = pos / rowHeight - rowStartIndex;
			if (pos % rowHeight > 0)
			{
				rowIndexToMove++;
			}
		}

		//3.4. 줄 수 만큼 반복한다.
		Long nearestIndex;
		Long rowCount = rowIndexToMove - rowIndex;
		Long i = 0;
		while (i < rowCount)
		{
			//3.4.1. 줄의 끝까지 반복한다.
			while (columnIndex < row->GetLength())
			{
				columnIndex = row->Next();
				pagingBuffer->Next();
			}

			//3.4.2. 다음줄로 이동한다.
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			if (!row->IsDummyRow())
			{
				pagingBuffer->NextRow();
			}

			//3.4.3. 가까운 위치까지 반복한다.
			nearestIndex = sizeCalculator->GetNearestColumnIndex(row, columnIndex);
			while (columnIndex < nearestIndex)
			{
				columnIndex = row->Next();
				pagingBuffer->Next();
			}
			i++;
		}
	}
}