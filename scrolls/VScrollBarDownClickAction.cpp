#include <afxwin.h>
#include "VScrollBarDownClickAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

VScrollBarDownClickAction::VScrollBarDownClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarDownClickAction::~VScrollBarDownClickAction() {

}

void VScrollBarDownClickAction::Perform() {
	//1. 스크롤을 한 줄만큼 내린다.
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

	//2. 현재 줄의 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 현재 줄의 위치가 보이는 영역을 벗어났으면,
	if (currentPos < pos)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}
}