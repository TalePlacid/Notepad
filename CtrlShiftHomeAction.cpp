#include <afxwin.h>
#include "CtrlShiftHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4966)

CtrlShiftHomeAction::CtrlShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftHomeAction::~CtrlShiftHomeAction() {

}

void CtrlShiftHomeAction::Perform() {
	//1. 노트에서 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄에서 현재 칸부터 시작 칸까지 반복한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex - 1;
	while (i >= 0)
	{
		//2.1. 문자의 선택여부에 따라 선택한다.
		character = row->GetAt(i);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			row->Previous();
			pagingBuffer->Previous();
		}
		else
		{
			character->Select(FALSE);
			row->Previous();
			pagingBuffer->Previous();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
		i--;
	}

	if (pagingBuffer->GetSelectionBeginOffset() < 0)
	{
		pagingBuffer->MarkSelectionBegin();
	}
	rowIndex = note->Previous();
	pagingBuffer->PreviousRow();
	if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
	{
		pagingBuffer->UnmarkSelectionBegin();
	}

	//3. 노트에서 처음 위치까지 반복한다.
	Long j;
	i = rowIndex;
	while (i >= 0)
	{
		//3.1. 문자의 선택여부에 따라 선택한다.
		row = note->GetAt(i);
		j = row->GetLength() - 1;
		while (j >= 0)
		{
			character = row->GetAt(j);
			if (!character->IsSelected())
			{
				character->Select(TRUE);
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
				row->Previous();
				pagingBuffer->Previous();
			}
			else
			{
				character->Select(FALSE);
				row->Previous();
				pagingBuffer->Previous();
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
			j--;
		}

		if (pagingBuffer->GetSelectionBeginOffset() < 0)
		{
			pagingBuffer->MarkSelectionBegin();
		}
		note->Previous();
		pagingBuffer->Previous();
		if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
		{
			pagingBuffer->UnmarkSelectionBegin();
		}

		i--;
	}

	//4. 첫 페이지가 아니면, 첫 페이지를 적재한다.
	if (pagingBuffer->GetStartOffset() > 0)
	{
		pagingBuffer->FirstRow();
		pagingBuffer->Load();
	}

	//5. 스크롤바를 조정한다.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

	//6. 클라이언트 영역을 갱신한다.
	this->parent->Invalidate();
}