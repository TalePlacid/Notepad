#include <afxwin.h>
#include "VScrollBarPageDownAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "CaretNavigator.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarPageDownAction::VScrollBarPageDownAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageDownAction::~VScrollBarPageDownAction() {

}

void VScrollBarPageDownAction::Perform() {
	//1. 스크롤 크기만큼 페이지를 내린다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() + vScroll.GetPage();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 현재위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 현재 줄의 위치가 보이는 영역을 벗어났으면,
	if (currentPos < pos)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretUpToVScroll(rowWidth);
	}
}