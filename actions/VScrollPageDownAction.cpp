#include <afxwin.h>
#include "VScrollPageDownAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../SizeCalculator.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollPageDownAction::VScrollPageDownAction(CWnd* parent)
	:Action(parent) {

}

VScrollPageDownAction::~VScrollPageDownAction() {

}

void VScrollPageDownAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() + vScroll.GetPage();
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

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowWidth = sizeCalculator->GetRowWidth(rowIndex, columnIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	if (currentPos < pos)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}
}

bool VScrollPageDownAction::NeedScrollBarUpdate() {
	return false;
}
