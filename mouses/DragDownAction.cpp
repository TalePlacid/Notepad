#include "DragDownAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../CoordinateConverter.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

DragDownAction::DragDownAction(CWnd* parent, CPoint point)
	:MouseAction(parent, point) {

}

DragDownAction::~DragDownAction() {

}

void DragDownAction::Perform() {
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
	Scroll vScroll = scrollController->GetVScroll();
	Long pageMax = vScroll.GetPos() + vScroll.GetPage();
	Long difference;
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

		//필요시 적재
		if (note->IsBelowBottomLine(currentRowIndex + 1) && pageMax < vScroll.GetPos())
		{
			difference = rowIndex - currentRowIndex;
			PageLoader::LoadNext(this->parent);
			currentRowIndex = note->GetCurrent();
			rowIndex = currentRowIndex + difference;
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
