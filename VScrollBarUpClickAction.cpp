#include <afxwin.h>
#include "VScrollBarUpClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarUpClickAction::VScrollBarUpClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarUpClickAction::~VScrollBarUpClickAction() {

}

void VScrollBarUpClickAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	Long rowHeight = sizeCalculator->GetRowHeight();
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() - rowHeight;
	if (pos < 0)
	{
		pos = 0;
	}
	scrollController->MoveVScroll(pos);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowIndexToMove = pos / rowHeight - pagingBuffer->GetRowStartIndex();

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	if (note->IsAboveTopLine(rowIndexToMove) && pagingBuffer->GetRowStartIndex() > 0)
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
		rowIndex = note->GetCurrent();
		rowIndexToMove = pos / rowHeight - pagingBuffer->GetRowStartIndex();
	}

	pagingBuffer->PreviousRow(rowIndex - rowIndexToMove);
	rowIndex = note->Move(rowIndexToMove);
	row = note->GetAt(rowIndex);
	columnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
	pagingBuffer->Next(columnIndex);
	columnIndex = row->Move(columnIndex);
}