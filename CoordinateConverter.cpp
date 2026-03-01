#include "CoordinateConverter.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "glyphs/Glyph.h"

#pragma warning(disable:4996)

CoordinateConverter::CoordinateConverter(CWnd* parent) {
	this->parent = parent;
}

CoordinateConverter::~CoordinateConverter() {

}

CPoint CoordinateConverter::DisplayToAbsolute(CPoint point) {
	CPoint absolutePoint = point;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (scrollController->HasVScroll())
	{
		absolutePoint.y += scrollController->GetVScroll().GetPos();
	}

	if (scrollController->HasHScroll())
	{
		absolutePoint.x += scrollController->GetHScroll().GetPos();
	}

	return absolutePoint;
}

void CoordinateConverter::AbsoluteToNotePosition(CPoint point, Long& rowIndex, Long& columnIndex) {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long fileRowIndex = point.y / rowHeight;
	rowIndex = fileRowIndex - rowStartIndex;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	if (rowIndex >= note->GetLength())
	{
		rowIndex = note->GetLength() - 1;
	}

	//3. 노트상의 칸위치를 구한다.
	Glyph* row = note->GetAt(rowIndex);
	columnIndex = sizeCalculator->GetNearestColumnIndex(row, point.x);
}
