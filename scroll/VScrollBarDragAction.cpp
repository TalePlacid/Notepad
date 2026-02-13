#include <afxwin.h>
#include "VScrollBarDragAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "CaretNavigator.h"
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
	CaretNavigator caretNavigator(this->parent);
	if (currentPos < pos)
	{
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}

	//5. 현재 줄의 위치가 스크롤 범위보다 아래라면,
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	if (currentPos + rowHeight > pos + page)
	{
		caretNavigator.AdjustCaretDownToVScroll(rowWidth);
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