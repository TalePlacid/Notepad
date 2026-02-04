#include "MouseHandler.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "Glyph.h"

#pragma warning(disable:4996)

MouseHandler::MouseHandler(CWnd* parent) {
	this->parent = parent;
}

MouseHandler::~MouseHandler() {

}

void MouseHandler::DownLeftButton(UINT nFlags, CPoint point) {
	//1. 절대좌표로 환산한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;

	CPoint absolutePoint = point;
	if (scrollController->HasVScroll())
	{
		absolutePoint.y += scrollController->GetVScroll().GetPos();
	}

	if (scrollController->HasHScroll())
	{
		absolutePoint.x += scrollController->GetHScroll().GetPos();
	}

	//2. 노트상의 줄위치를 구한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long fileRowIndex = absolutePoint.y / rowHeight;
	Long noteRowIndex = fileRowIndex - rowStartIndex;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	if (noteRowIndex >= note->GetLength())
	{
		noteRowIndex = note->GetLength() - 1;
	}

	//3. 노트상의 칸위치를 구한다.
	Glyph* row = note->GetAt(noteRowIndex);
	Long columnIndex = sizeCalculator->GetNearestColumnIndex(row, absolutePoint.x);

	//4. 지금 줄의 가장 앞으로 이동한다.
	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();
	currentRow->First();
	pagingBuffer->Previous(currentColumnIndex);

	//5. 현재 위치보다 앞이면,
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
	else //6. 현재 위치보다 뒤이면,
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

	//7. 칸 위치로 이동한다.
	currentRow->Move(columnIndex);
	pagingBuffer->Next(columnIndex);

	//8. 클라이언트 영역을 갱신한다.
	this->parent->Invalidate();
}