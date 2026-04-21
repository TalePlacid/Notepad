#include <afxwin.h>
#include "VScrollPageUpAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"
#include "../SizeCalculator.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollPageUpAction::VScrollPageUpAction(CWnd* parent)
	:Action(parent) {

}

VScrollPageUpAction::~VScrollPageUpAction() {

}

void VScrollPageUpAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	Long pos = vScroll.GetPos() - page;
	if (pos < 0)
	{
		pos = 0;
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

	if (currentPos + rowHeight > pos + page)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretDownToVScroll(rowWidth);
	}
}

bool VScrollPageUpAction::NeedScrollBarUpdate() {
	return false;
}
