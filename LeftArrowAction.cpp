#include <afxwin.h>
#include "LeftArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

LeftArrowAction::LeftArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

LeftArrowAction::~LeftArrowAction() {

}

void LeftArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//1. 줄의 처음이 아니라면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (columnIndex > 0)
	{
		//1.1. 노트에서 이전 칸으로 이동한다.
		columnIndex = row->Previous();

		//1.2. 페이징버퍼에서 이전 칸으로 이동한다.
		pagingBuffer->Previous();
	}
	else //2. 줄의 처음이라면,
	{
		//2.1. 이전 줄이 유효범위를 넘어서면, 적재한다.
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		//2.2. 첫번째 줄이 아니라면,
		if (rowIndex > 0)
		{
			//2.2.1. 노트에서 이전 줄로 이동한다.
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->Last();

			//2.2.2. 페이징 버퍼에서 이전줄로 이동한다.
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
		}
	}
	note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
}