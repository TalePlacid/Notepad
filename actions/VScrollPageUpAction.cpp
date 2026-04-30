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
	//1. 스크롤 위치를 이동한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	Long pos = vScroll.GetPos() - page;
	if (pos < 0)
	{
		pos = 0;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 화면 하단 절대줄을 구한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
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

bool VScrollPageUpAction::NeedScrollBarUpdate() {
	return false;
}
