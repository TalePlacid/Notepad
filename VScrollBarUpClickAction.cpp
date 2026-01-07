#include <afxwin.h>
#include "VScrollBarUpClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "UpArrowAction.h"
#include "resource.h"

#pragma warning(disable:4996)

VScrollBarUpClickAction::VScrollBarUpClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarUpClickAction::~VScrollBarUpClickAction() {

}

void VScrollBarUpClickAction::Perform() {
	//1. 스크롤을 한 줄 올린다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos() - rowHeight;
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

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long currentPos = (rowStartIndex + rowIndex) * rowHeight;

	//3. 보이는 영역을 벗어났다면, 한 줄 올린다.
	if (currentPos + rowHeight > pos + vScroll.GetPage())
	{
		UpArrowAction upArrowAction(this->parent);
		upArrowAction.Perform();
	}
}