#include "NotePositionResolver.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "Glyph.h"

#pragma warning(disable:4996)

NotePositionResolver::NotePositionResolver(CWnd* parent) {
	this->parent = parent;
}

NotePositionResolver::~NotePositionResolver() {

}

void NotePositionResolver::PointToNotePosition(CPoint point, Long& rowIndex, Long& columnIndex) {
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
	rowIndex = fileRowIndex - rowStartIndex;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	if (rowIndex >= note->GetLength())
	{
		rowIndex = note->GetLength() - 1;
	}

	//3. 노트상의 칸위치를 구한다.
	Glyph* row = note->GetAt(rowIndex);
	columnIndex = sizeCalculator->GetNearestColumnIndex(row, absolutePoint.x);
}