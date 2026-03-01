#include <afxwin.h>
#include "VScrollBarDragAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollBarDragAction::VScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	this->nPos = scrollInfo.nTrackPos;
}

VScrollBarDragAction::~VScrollBarDragAction() {

}

void VScrollBarDragAction::Perform() {
	//1. 스크롤을 이동한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long pos = scrollController->MoveVScroll(this->nPos);

	//2. 현재줄의 위치를 구한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();

	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 현재 줄 너비를 구한다.
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	//4. 현재 줄의 위치가 스크롤 범위보다 위라면,
	CaretNavigator caretNavigator(this->parent);
	if (currentPos < pos)
	{
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}

	//5. 현재 줄의 위치가 스크롤 범위보다 아래라면,
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	if (currentPos + rowHeight > pos + page)
	{
		caretNavigator.AdjustCaretDownToVScroll(rowWidth);
	}
}
