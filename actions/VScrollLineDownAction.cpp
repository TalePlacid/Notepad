#include <afxwin.h>
#include "VScrollLineDownAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollLineDownAction::VScrollLineDownAction(CWnd* parent)
	:Action(parent) {

}

VScrollLineDownAction::~VScrollLineDownAction() {

}

void VScrollLineDownAction::Perform() {
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
	pos = scrollController->MoveVScroll(pos);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	if (currentPos < pos)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}
}

bool VScrollLineDownAction::NeedScrollBarUpdate() {
	return false;
}
