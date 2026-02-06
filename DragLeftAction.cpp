#include "DragLeftAction.h"
#include "NotepadForm.h"
#include "NotePositionResolver.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(diable:4996)

DragLeftAction::DragLeftAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragLeftAction::~DragLeftAction() {

}

void DragLeftAction::Perform() {
	NotePositionResolver notePositionResolver(this->parent);
	Long rowIndex;
	Long columnIndex;
	notePositionResolver.PointToNotePosition(this->point, rowIndex, columnIndex);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (currentColumnIndex > columnIndex)
	{
		pagingBuffer->BeginSelectionIfNeeded();
		currentColumnIndex = row->Previous();
		row->GetAt(currentColumnIndex)->ToggleSelection();
		pagingBuffer->Previous();
		pagingBuffer->EndSelectionIfCollapsed();
	}
}