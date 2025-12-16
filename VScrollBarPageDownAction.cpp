#include <afxwin.h>
#include "VScrollBarPageDownAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarPageDownAction::VScrollBarPageDownAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageDownAction::~VScrollBarPageDownAction() {

}

void VScrollBarPageDownAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() + vScroll.GetPage();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	pos = scrollController->MoveVScroll(pos);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
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