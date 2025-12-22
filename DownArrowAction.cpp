#include <afxwin.h>
#include "DownArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

DownArrowAction::DownArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

DownArrowAction::~DownArrowAction() {

}

void DownArrowAction::Perform() {
	//1. 다음 줄이 재적재범위에 들어가면, 재적재한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	if (note->IsBelowBottomLine(rowIndex + 1) && vScroll.GetPos() + vScroll.GetPage() < vScroll.GetMax())
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
	}

	//2. 다음 줄로 이동한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Glyph* originalRow = note->GetAt(rowIndex);
	Long columnIndex = originalRow->GetCurrent();
	Long originalWidth = sizeCalculator->GetRowWidth(originalRow, columnIndex);

	rowIndex = note->Next();
	Glyph* nextRow = note->GetAt(rowIndex);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (!nextRow->IsDummyRow())
	{
		pagingBuffer->NextRow();
	}

	Long nearestIndex = sizeCalculator->GetNearestColumnIndex(nextRow, originalWidth);
	nextRow->Move(nearestIndex);


	if (!nextRow->IsDummyRow())
	{
		pagingBuffer->Move(nearestIndex);
	}
	else
	{
		pagingBuffer->Next(nearestIndex);
	}
}