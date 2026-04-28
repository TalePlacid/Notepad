#include "MoveToPointAction.h"
#include "../NotepadForm.h"
#include "../MouseHandler.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../CoordinateConverter.h"

#pragma warning(disable:4996)

MoveToPointAction::MoveToPointAction(CWnd* parent, CPoint point)
	:Action(parent), point(point) {

}

MoveToPointAction::~MoveToPointAction() {

}

void MoveToPointAction::Perform() {
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

	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long bytes = currentRow->GetPreviousBytes(currentColumnIndex);
	currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

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
				currentOffset = pagingBuffer->MoveOffset(currentOffset - 2);
			}

			//첫번째 위치로 이동
			currentRow->First();
			bytes = currentRow->GetPreviousBytes(currentColumnIndex);
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
		}
	}
	else //5. 현재 위치보다 뒤이면,
	{
		Long previousColumnIndex;
		while (currentRowIndex < rowIndex)
		{
			previousRow = currentRow;
			previousColumnIndex = previousRow->GetCurrent();
			currentRowIndex = note->Next();
			currentRow = note->GetAt(currentRowIndex);
			currentRow->First();

			bytes = previousRow->GetNextBytes(previousColumnIndex);
			if (!currentRow->IsDummyRow())
			{
				bytes += 2;
			}
			currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
		}
	}

	//6. 칸 위치로 이동한다.
	columnIndex = currentRow->Move(columnIndex);
	bytes = currentRow->GetPreviousBytes(columnIndex);
	currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
}