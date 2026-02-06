#include <afxwin.h>
#include "DragUpAction.h"
#include "NotepadForm.h"
#include "NotePositionResolver.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

DragUpAction::DragUpAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragUpAction::~DragUpAction() {

}

void DragUpAction::Perform() {
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
	while (currentRowIndex > rowIndex)
	{
		//현재줄
		while (currentColumnIndex > 0)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentColumnIndex = row->Previous();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//줄이동
		previousRow = row;
		currentRowIndex = note->Previous();
		row = note->GetAt(currentRowIndex);
		currentColumnIndex = row->Last();
		if (!previousRow->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
			pagingBuffer->EndSelectionIfCollapsed();
		}

		//이전줄
		while (currentColumnIndex > columnIndex)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentColumnIndex = row->Previous();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}