#include <afxwin.h>
#include "RightArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "MarkingHelper.h"
#include "resource.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

RightArrowAction::RightArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

RightArrowAction::~RightArrowAction() {

}

void RightArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//1. 줄의 끝이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		columnIndex = row->Next();
		pagingBuffer->Next();
	}
	else
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * sizeCalculator->GetRowHeight();
		if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}

		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			Glyph* nextRow = note->GetAt(rowIndex);
			columnIndex = nextRow->First();

			if (!nextRow->IsDummyRow())
			{
				pagingBuffer->NextRow();
			}
		}
	}
}