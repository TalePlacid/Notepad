#include <afxwin.h>
#include "VScrollBarDownClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarDownClickAction::VScrollBarDownClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarDownClickAction::~VScrollBarDownClickAction() {

}

void VScrollBarDownClickAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	Scroll vScroll = scrollController->GetVScroll();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long pos = vScroll.GetPos() + rowHeight;
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	scrollController->MoveVScroll(pos);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowIndexToMove = pos / rowHeight - pagingBuffer->GetRowStartIndex();

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	if (note->IsBelowBottomLine(rowIndexToMove))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
		rowIndexToMove = pos / rowHeight - pagingBuffer->GetRowStartIndex();
	}

	pagingBuffer->NextRow(rowIndexToMove - rowIndex);
	rowIndex = note->Move(rowIndexToMove);
	row = note->GetAt(rowIndex);
	columnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
	pagingBuffer->Next(columnIndex);
	columnIndex = row->Move(columnIndex);
}