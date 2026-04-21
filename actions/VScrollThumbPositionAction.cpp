#include <afxwin.h>
#include "VScrollThumbPositionAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollThumbPositionAction::VScrollThumbPositionAction(CWnd* parent)
	:Action(parent) {

}

VScrollThumbPositionAction::~VScrollThumbPositionAction() {

}

void VScrollThumbPositionAction::Perform() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long pos = scrollController->MoveVScroll(scrollInfo.nPos);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long absoluteRowIndex = pos / rowHeight;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(currentRowIndex, currentColumnIndex);

	Long currentAbsoluteRowIndex = pagingBuffer->GetRowStartIndex() + currentRowIndex;
	CaretNavigator caretNavigator(this->parent);
	if (absoluteRowIndex < currentAbsoluteRowIndex)
	{
		caretNavigator.MoveCaretUpToAbsoluteRow(absoluteRowIndex, rowWidth);
	}
	else if (absoluteRowIndex > currentAbsoluteRowIndex)
	{
		caretNavigator.MoveCaretDownToAbsoluteRow(absoluteRowIndex, rowWidth);
	}
}

bool VScrollThumbPositionAction::NeedScrollBarUpdate() {
	return false;
}