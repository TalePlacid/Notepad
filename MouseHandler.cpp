#include "MouseHandler.h"
#include "NotePositionResolver.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "VScrollBarUpClickAction.h"
#include "vScrollBarDownClickAction.h"
#include "resource.h"

#pragma warning(disable:4996)

MouseHandler::MouseHandler(CWnd* parent)
	:latestPoint(-1, -1) {
	this->parent = parent;
	this->onDrag = FALSE;                                                                                    
}

MouseHandler::~MouseHandler() {

}

CPoint MouseHandler::DownLeftButton(CPoint point) {
	//1. 선택을 해제한다.
	((NotepadForm*)(this->parent))->note->Select(false);
	((NotepadForm*)(this->parent))->pagingBuffer->UnmarkSelectionBegin();
	this->latestPoint = point;
	this->onDrag = TRUE;

	//2. 좌표를 노트 위치로 바꾼다.
	NotePositionResolver notePositionResolver(this->parent);
	Long noteRowIndex;
	Long columnIndex;
	notePositionResolver.PointToNotePosition(this->latestPoint, noteRowIndex, columnIndex);

	//3. 지금 줄의 가장 앞으로 이동한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();
	currentRow->First();
	pagingBuffer->Previous(currentColumnIndex);

	//4. 현재 위치보다 앞이면,
	Glyph* previousRow;
	if (currentRowIndex > noteRowIndex)
	{
		while (currentRowIndex > noteRowIndex)
		{
			//줄이동
			previousRow = currentRow;
			currentRowIndex = note->Previous();
			currentRow = note->GetAt(currentRowIndex);
			currentColumnIndex = currentRow->Last();
			if (!previousRow->IsDummyRow())
			{
				pagingBuffer->PreviousRow();
				pagingBuffer->Last();
			}

			//첫번째 위치로 이동
			currentRow->First();
			pagingBuffer->Previous(currentRow->GetLength());
		}
	}
	else //5. 현재 위치보다 뒤이면,
	{
		while (currentRowIndex < noteRowIndex)
		{
			previousRow = currentRow;
			currentRowIndex = note->Next();
			currentRow = note->GetAt(currentRowIndex);
			currentRow->First();
			if (currentRow->IsDummyRow())
			{
				pagingBuffer->Next(previousRow->GetLength());
			}
			else
			{
				pagingBuffer->NextRow();
			}
		}
	}

	//6. 칸 위치로 이동한다.
	currentRow->Move(columnIndex);
	pagingBuffer->Next(columnIndex);

	//7. 클라이언트 영역을 갱신한다.
	this->parent->Invalidate();

	return this->latestPoint;
}

void MouseHandler::EndDrag(CPoint point) {
	this->onDrag = FALSE;
	this->latestPoint.SetPoint(-1, -1);
}

BOOL MouseHandler::WheelMouse(short zDelta) {
	//1. 수직스크롤이 존재한다면,
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	BOOL hasVScroll = notepadForm->scrollController->HasVScroll();
	if (hasVScroll)
	{
		//1.1. 방향을 확인한다.
		BOOL isUp = TRUE;
		if (zDelta < 0)
		{
			isUp = FALSE;
			zDelta *= -1;
		}

		//1.2. 반복 횟수를 정한다.
		Long count = zDelta / DELTA_PER_TICK;

		//1.3. 방향에 따라 반복한다.
		ScrollBarAction* action = NULL;
		if (isUp)
		{
			action = new VScrollBarUpClickAction(this->parent);
		}
		else
		{
			action = new VScrollBarDownClickAction(this->parent);
		}

		for (Long i = 0; i < count; i++)
		{
			action->Perform();
		}

		if (action != NULL)
		{
			delete action;
		}

		notepadForm->note->Select(false);
		notepadForm->pagingBuffer->UnmarkSelectionBegin();
		notepadForm->Notify("UpdateScrollBars");
		notepadForm->Notify("UpdateStatusBar");
		notepadForm->Invalidate();
	}

	return hasVScroll;
}

void MouseHandler::CtrlWheelMouse(short zDelta) {
	//1. 방향을 확인한다.
	UINT nID = ID_MENU_ZOOMIN;
	if (zDelta < 0)
	{
		nID = ID_MENU_ZOOMOUT;
		zDelta *= -1;
	}

	//2. 반복 횟수를 정한다.
	Long count = zDelta / DELTA_PER_TICK;

	//3. 방향에 따라 반복한다.
	for (Long i = 0; i < count; i++)
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)nID, 0);
	}
}

CPoint& MouseHandler::UpdateLatestPoint(CPoint point) {
	this->latestPoint = point;

	return this->latestPoint;
}

BOOL MouseHandler::IsMovedAboveRow(CPoint point) {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long rowStart = this->latestPoint.y / rowHeight * rowHeight;

	BOOL ret = FALSE;
	if (point.y < rowStart)
	{
		ret = TRUE;
	}

	return ret;
}

BOOL MouseHandler::IsMovedBelowRow(CPoint point) {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long rowEnd = (this->latestPoint.y / rowHeight + 1) * rowHeight;

	BOOL ret = FALSE;
	if (point.y > rowEnd)
	{
		ret = TRUE;
	}

	return ret;
}

BOOL MouseHandler::IsMovedLeft(CPoint point) {
	BOOL ret = FALSE;
	Long xDifference = point.x - this->latestPoint.x;
	if (xDifference <= -4)
	{
		ret = TRUE;
	}

	return ret;
}

BOOL MouseHandler::IsMovedRight(CPoint point) {
	BOOL ret = FALSE;
	Long xDifference = point.x - this->latestPoint.x;
	if (xDifference >= 4)
	{
		ret = TRUE;
	}

	return ret;
}