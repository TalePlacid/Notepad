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
	//1. 스크롤 위치를 이동한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() + vScroll.GetPage();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 화면 상단 절대줄위치를 구한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Long absoluteAboveIndex = pos / rowHeight;
	if (pos % rowHeight > 0)
	{
		absoluteAboveIndex++;
	}

	//3. 현재 줄위치가 화면 상단 절대줄보다 이전이면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Long absoluteRowIndex = rowIndex + pagingBuffer->GetRowStartIndex();
	if (absoluteRowIndex < absoluteAboveIndex)
	{
		//3.1. 화면 상단 줄로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretUpToVScroll();
	}
}

bool VScrollPageDownAction::NeedScrollBarUpdate() {
	return false;
}
