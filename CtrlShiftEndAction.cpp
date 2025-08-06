#include <afxwin.h>
#include "CtrlShiftEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

CtrlShiftEndAction::CtrlShiftEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftEndAction::~CtrlShiftEndAction() {

}

void CtrlShiftEndAction::Perform() {
	//1. 노트에서 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄에서 현재 칸부터 끝까지 반복한다.
	MarkingHelper markingHelper(this->parent);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex;
	while (i < row->GetLength())
	{
		//2.1. 문자의 선택여부에 따라 선택한다.
		character = row->GetAt(i);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (markingHelper.IsUnmarked())
			{
				markingHelper.Mark();
			}
			row->Next();
			pagingBuffer->Next();
		}
		else
		{
			character->Select(FALSE);
			row->Next();
			pagingBuffer->Next();
			if (markingHelper.HasReturnedToSelectionBegin())
			{
				markingHelper.Unmark();
			}
		}
		i++;
	}

	if (markingHelper.IsUnmarked())
	{
		markingHelper.Mark();
	}
	rowIndex = note->Next();
	pagingBuffer->NextRow();
	if (markingHelper.HasReturnedToSelectionBegin())
	{
		markingHelper.Unmark();
	}

	//3. 노트에서 끝까지 반복한다.
	Long j;
	i = rowIndex;
	while (i < note->GetLength())
	{
		//3.1. 문자의 선택여부에 따라 선택한다.
		row = note->GetAt(i);
		j = 0;
		while (j < row->GetLength())
		{
			character = row->GetAt(j);
			if (!character->IsSelected())
			{
				character->Select(TRUE);
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}
				row->Next();
				pagingBuffer->Next();
			}
			else
			{
				character->Select(FALSE);
				row->Next();
				pagingBuffer->Next();
				if (markingHelper.HasReturnedToSelectionBegin())
				{
					markingHelper.Unmark();
				}
			}
			j++;
		}

		if (markingHelper.IsUnmarked())
		{
			markingHelper.Mark();
		}
		note->Next();
		pagingBuffer->NextRow();
		if (markingHelper.HasReturnedToSelectionBegin())
		{
			markingHelper.Unmark();
		}
		i++;
	}

	//4. 마지막 페이지가 아니라면, 마지막 페이지로 이동한다.
	if (pagingBuffer->GetEndOffset() < pagingBuffer->GetFileEndOffset())
	{
		pagingBuffer->LastRow();
		pagingBuffer->Last();
		pagingBuffer->Load();
	}

	//5. 스크롤바를 조정한다.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

	//6. 클라이언트 영역을 갱신한다.
	parent->Invalidate();
}