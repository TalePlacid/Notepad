#include <afxwin.h>
#include "VScrollBarDragAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarDragAction::VScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	this->nPos = scrollInfo.nTrackPos;
}

VScrollBarDragAction::~VScrollBarDragAction() {

}

void VScrollBarDragAction::Perform() {
	//1. 스크롤을 이동한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long pos = scrollController->MoveVScroll(this->nPos);

	//2. 현재줄의 위치를 구한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();

	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 현재 줄 너비를 구한다.
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	//4. 현재 줄의 위치가 스크롤 범위보다 위라면,
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	if (currentPos + rowHeight > pos + page)
	{
		//4.1. 화면 끝에 해당하는 줄 위치를 찾는다.
		Long rowIndexToMove = (pos + page) / rowHeight - rowStartIndex - 1;
		if (rowIndexToMove < 0)
		{
			rowIndexToMove = 0;
		}

		//4.2. 적재범위를 넘어섰으면, 재적재한다.
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

		//4.3. 줄 수 만큼 반복한다.
		Long nearestIndex;
		Glyph* previousRow;
		Long rowCount = rowIndex - rowIndexToMove;
		Long i = 0;
		while (i < rowCount && rowIndex > 0)
		{
			//4.3.1. 줄의 처음까지 반복한다.
			row = note->GetAt(rowIndex);
			while (columnIndex > 0)
			{
				columnIndex = row->Previous();
				pagingBuffer->Previous();
			}

			//4.3.2. 윗 줄로 이동한다.
			previousRow = row;
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->Last();

			if (!previousRow->IsDummyRow())
			{
				pagingBuffer->PreviousRow();
				pagingBuffer->Last();
			}

			//4.3.3. 가까운 위치까지 반복한다.
			nearestIndex = sizeCalculator->GetNearestColumnIndex(row, columnIndex);
			while (columnIndex > nearestIndex)
			{
				columnIndex = row->Previous();
				pagingBuffer->Previous();
			}

			i++;
		}
	}

	//5. 현재 줄의 위치가 스크롤 범위보다 아래라면,
	if (currentPos < pos)
	{
		//5.1. 스크롤에 해당하는 줄 위치를 구한다.
		Long rowIndexToMove = pos / rowHeight - rowStartIndex;
		if (pos % rowHeight > 0)
		{
			rowIndexToMove++;
		}

		//5.2. 적재범위를 벗어나면, 재적재한다.
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

		//5.3. 줄 수 만큼 반복한다.
		Long nearestIndex;
		Long rowCount = rowIndexToMove - rowIndex;
		Long i = 0;
		while (i < rowCount && rowIndex + 1 < note->GetLength())
		{
			//5.3.1. 줄의 끝까지 반복한다.
			while (columnIndex < row->GetLength())
			{
				columnIndex = row->Next();
				pagingBuffer->Next();
			}

			//5.3.2. 다음줄로 이동한다.
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			if (!row->IsDummyRow())
			{
				pagingBuffer->NextRow();
			}

			//5.3.3. 가까운 위치까지 반복한다.
			nearestIndex = sizeCalculator->GetNearestColumnIndex(row, columnIndex);
			while (columnIndex < nearestIndex)
			{
				columnIndex = row->Next();
				pagingBuffer->Next();
			}
			i++;
		}
	}

#if 0
	//1. 현재 줄의 너비를 구한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	//2. 현재 페이지의 pos범위를 구한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long pageStart = pagingBuffer->GetRowStartIndex() * rowHeight;
	Long pageEnd = pageStart + note->GetLength() * rowHeight;

	//3. 페이지끝이 추적위치보다 작다면 반복한다.
	while (pageEnd < this->nPos)
	{
		//3.1. 페이지의 가장 아랫줄로 이동한다.
		pagingBuffer->NextRow(note->GetLength() - (rowIndex + 1));
		pagingBuffer->First();
		rowIndex = note->Move(note->GetLength() - 1);
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		//3.2. 아랫부분을 재적재한다.
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
		pageStart = pagingBuffer->GetRowStartIndex() * rowHeight;
		pageEnd = pageStart + note->GetLength() * rowHeight;
	}

	//4. 페이지시작이 추적위치보다 크다면 반복한다.
	while (pageStart > this->nPos)
	{
		//4.1. 페이지의 가장 윗줄로 이동한다.
		pagingBuffer->PreviousRow(rowIndex);
		pagingBuffer->First();
		rowIndex = note->Move(0);
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		//4.2. 윗부분을 재적재한다.
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
		rowIndex = note->GetCurrent();
		pageStart = pagingBuffer->GetRowStartIndex() * rowHeight;
		pageEnd = pageStart + note->GetLength() * rowHeight;
	}

	//5. 위치로 이동한다.
	Long rowIndexToMove = this->nPos / rowHeight - pagingBuffer->GetRowStartIndex();

	if (rowIndex < rowIndexToMove)
	{
		pagingBuffer->NextRow(rowIndexToMove - rowIndex);
	}
	else if (rowIndex > rowIndexToMove)
	{
		pagingBuffer->PreviousRow(rowIndex - rowIndexToMove);
	}

	rowIndex = note->Move(rowIndexToMove);
	row = note->GetAt(rowIndex);
	columnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
	pagingBuffer->Next(columnIndex);
	columnIndex = row->Move(columnIndex);

	//6. 적재범위를 넘어섰다면 재적재한다.
	if (note->IsAboveTopLine(rowIndex))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
	}
	else if (note->IsBelowBottomLine(rowIndex))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
	}

	//7. 스크롤을 조정한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	scrollController->MoveVScroll(this->nPos);
#endif
}