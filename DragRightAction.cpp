#include "DragRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "NotePositionResolver.h"

#pragma warning(disable:4996)

DragRightAction::DragRightAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragRightAction::~DragRightAction() {

}

void DragRightAction::Perform() {
	NotePositionResolver notePositionResolver(this->parent);
	Long rowIndex;
	Long columnIndex;
	notePositionResolver.PointToNotePosition(this->point, rowIndex, columnIndex);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (currentColumnIndex < columnIndex)
	{
		pagingBuffer->BeginSelectionIfNeeded();
		row->GetAt(currentColumnIndex)->ToggleSelection();
		currentColumnIndex = row->Next();
		pagingBuffer->Next();
		pagingBuffer->EndSelectionIfCollapsed();
	}
}