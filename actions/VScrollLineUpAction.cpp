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
	//1. 스크롤 위치를 이동한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() - rowHeight * 3;
	if (pos < 0)
	{
		pos = 0;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 화면 하단 절대줄을 구한다.
	Long endPos = pos + vScroll.GetPage();
	Long absoluteBelowIndex = endPos / rowHeight - 1;
	if (absoluteBelowIndex < 0)
	{
		absoluteBelowIndex = 0;
	}

	//3. 현재 줄이 화면 하단 절대줄보다 아래이면, 화면 하단 절대줄로 이동한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();

	Long absoluteRowIndex = rowStartIndex + rowIndex;
	if (absoluteRowIndex > absoluteBelowIndex)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretDownToVScroll();
	}
}

bool VScrollLineUpAction::NeedScrollBarUpdate() {
	return false;
}
