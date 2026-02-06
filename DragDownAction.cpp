#include "DragDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NotePositionResolver.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

DragDownAction::DragDownAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragDownAction::~DragDownAction() {

}

void DragDownAction::Perform() {
	NotePositionResolver notePositionResolver(this->parent);
	Long rowIndex;
	Long columnIndex;
	notePositionResolver.PointToNotePosition(this->point, rowIndex, columnIndex);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();

	Glyph* previousRow;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (currentRowIndex < rowIndex)
	{
		//현재줄
		while (currentColumnIndex < row->GetLength())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			currentColumnIndex = row->Next();
			pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//줄이동
		previousRow = row;
		currentRowIndex = note->Next();
		row = note->GetAt(currentRowIndex);
		currentColumnIndex = row->First();
		if (!row->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->NextRow();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//다음줄
		while (currentColumnIndex < columnIndex && currentColumnIndex < row->GetLength())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			currentColumnIndex = row->Next();
			pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}

}