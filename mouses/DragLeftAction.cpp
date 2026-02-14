#include "DragLeftAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../CoordinateConverter.h"

#pragma warning(diable:4996)

DragLeftAction::DragLeftAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragLeftAction::~DragLeftAction() {

}

void DragLeftAction::Perform() {
	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(this->point, rowIndex, columnIndex);

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