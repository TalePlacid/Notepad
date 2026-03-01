#include "MouseLeftDownAction.h"
#include "../NotepadForm.h"
#include "../MouseHandler.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../CoordinateConverter.h"

#pragma warning(disable:4996)

MouseLeftDownAction::MouseLeftDownAction(CWnd* parent, CPoint point) 
	:MouseAction(parent, point) {
}

MouseLeftDownAction::~MouseLeftDownAction() {

}

void MouseLeftDownAction::Perform() {
	//1. 드래그를 시작한다.
	MouseHandler* mouseHandler = ((NotepadForm*)(this->parent))->mouseHandler;
	mouseHandler->StartDrag();

	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(this->point, rowIndex, columnIndex);

	//2. 지금 줄의 가장 앞으로 이동한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();
	currentRow->First();
	pagingBuffer->Previous(currentColumnIndex);

	//4. 현재 위치보다 앞이면,
	Glyph* previousRow;
	if (currentRowIndex > rowIndex)
	{
		while (currentRowIndex > rowIndex)
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
		while (currentRowIndex < rowIndex)
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
}
