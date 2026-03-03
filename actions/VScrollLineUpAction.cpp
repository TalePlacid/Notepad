#include <afxwin.h>
#include "VScrollLineUpAction.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollLineUpAction::VScrollLineUpAction(CWnd* parent)
	:Action(parent) {

}

VScrollLineUpAction::~VScrollLineUpAction() {

}

void VScrollLineUpAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() - rowHeight;
	if (pos < 0)
	{
		pos = 0;
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

	if (currentPos + rowHeight > pos + vScroll.GetPage())
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretDownToVScroll(rowWidth);
	}
}

bool VScrollLineUpAction::NeedScrollBarUpdate() {
	return false;
}
