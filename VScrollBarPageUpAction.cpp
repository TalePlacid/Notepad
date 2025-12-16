#include <afxwin.h>
#include "VScrollBarPageUpAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "resource.h"
#include "Note.h"

#pragma warning(disable:4996)

VScrollBarPageUpAction::VScrollBarPageUpAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageUpAction::~VScrollBarPageUpAction() {

}

void VScrollBarPageUpAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() - vScroll.GetPage();
	if (pos < 0)
	{
		pos = 0;
	}
	scrollController->MoveVScroll(pos);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
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