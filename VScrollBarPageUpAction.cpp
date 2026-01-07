#include <afxwin.h>
#include "VScrollBarPageUpAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "CaretNavigator.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarPageUpAction::VScrollBarPageUpAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageUpAction::~VScrollBarPageUpAction() {

}

void VScrollBarPageUpAction::Perform() {
	//1. 스크롤을 한 줄 올린다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long page = vScroll.GetPage();
	Long pos = vScroll.GetPos() - page;
	if (pos < 0)
	{
		pos = 0;
	}
	pos = scrollController->MoveVScroll(pos);

	//2. 현재 위치를 읽는다.
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

	//3. 보이는 영역을 벗어났다면,
	if (currentPos + rowHeight > pos + page)
	{
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.AdjustCaretDownToVScroll(rowWidth);
	}
}