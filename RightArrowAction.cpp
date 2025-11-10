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

	//1. 줄의 마지막이 아니라면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (columnIndex < row->GetLength())
	{
		//1.1. 노트에서 다음 칸으로 이동한다.
		columnIndex = row->Next();

		//1.2. 페이징버퍼에서 다음으로 이동한다.
		pagingBuffer->Next();

		//1.3. 수평스크롤 범위를 넘어갔다면, 조정한다.
		if (!scrollController->IsOnHScrollRange())
		{
			Long characterWidth = 0;
			if (columnIndex < row->GetLength() - 1)
			{
				characterWidth = sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(columnIndex)));
			}
			scrollController->Right(characterWidth);
		}
	}
	else //2. 줄의 마지막이라면,
	{
		//2.1. 다음 줄이 유효범위를 벗어난 다면 재적재한다.
		if (note->IsBelowBottomLine(rowIndex + 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();
		}

		//2.2. 마지막 줄이 아니라면,
		if (rowIndex < note->GetLength() - 1)
		{
			//2.2.1. 노트에서 다음 줄로 이동한다.
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			scrollController->MoveHScroll(0);

			//2.2.2. 페이징 버퍼에서 다음 줄로 이동한다.
			pagingBuffer->NextRow();

			//2.2.3. 수직스크롤이 넘어갔다면, 조정한다.
			if (!scrollController->IsOnVScrollRange())
			{
				scrollController->Down();
			}
		}
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
	this->parent->Invalidate();
}