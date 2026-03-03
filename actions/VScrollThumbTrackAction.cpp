#include <afxwin.h>
#include "VScrollThumbTrackAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollThumbTrackAction::VScrollThumbTrackAction(CWnd* parent)
	:Action(parent) {

}

VScrollThumbTrackAction::~VScrollThumbTrackAction() {

}

void VScrollThumbTrackAction::Perform() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long pos = scrollController->MoveVScroll(scrollInfo.nTrackPos);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();

	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	CaretNavigator caretNavigator(this->parent);
	if (currentPos < pos)
	{
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}

	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	if (currentPos + rowHeight > pos + page)
	{
		caretNavigator.AdjustCaretDownToVScroll(rowWidth);
	}
}

bool VScrollThumbTrackAction::NeedScrollBarUpdate() {
	return false;
}
