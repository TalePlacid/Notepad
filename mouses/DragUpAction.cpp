#include "DragUpAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../CoordinateConverter.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

DragUpAction::DragUpAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragUpAction::~DragUpAction() {

}

void DragUpAction::Perform() {
	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(this->point, rowIndex, columnIndex);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();

	Glyph* previousRow;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll;
	Long difference;
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

		//필요시 적재
		if (note->IsAboveTopLine(currentRowIndex) && vScroll.GetPos() > 0)
		{
			difference = currentRowIndex - rowIndex;
			PageLoader::LoadPrevious(this->parent);
			currentRowIndex = note->GetCurrent();
			rowIndex = currentRowIndex - difference;
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