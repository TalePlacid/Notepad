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
	//1. 수직 스크롤 위치를 내린다.
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

	//2. 화면 상단 절대줄을 구한다.
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

bool VScrollLineDownAction::NeedScrollBarUpdate() {
	return false;
}
