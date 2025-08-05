#include <afxwin.h>
#include "CtrlShiftEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

CtrlShiftEndAction::CtrlShiftEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftEndAction::~CtrlShiftEndAction() {

}

void CtrlShiftEndAction::Perform() {
	//1. ��Ʈ���� ���� ��ġ�� �д´�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. �ٿ��� ���� ĭ���� ������ �ݺ��Ѵ�.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex;
	while (i < row->GetLength())
	{
		//2.1. ������ ���ÿ��ο� ���� �����Ѵ�.
		character = row->GetAt(i);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			row->Next();
			pagingBuffer->Next();
		}
		else
		{
			character->Select(FALSE);
			row->Next();
			pagingBuffer->Next();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
		i++;
	}

	if (pagingBuffer->GetSelectionBeginOffset() < 0)
	{
		pagingBuffer->MarkSelectionBegin();
	}
	rowIndex = note->Next();
	pagingBuffer->NextRow();
	if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
	{
		pagingBuffer->UnmarkSelectionBegin();
	}

	//3. ��Ʈ���� ������ �ݺ��Ѵ�.
	Long j;
	i = rowIndex;
	while (i < note->GetLength())
	{
		//3.1. ������ ���ÿ��ο� ���� �����Ѵ�.
		row = note->GetAt(i);
		j = 0;
		while (j < row->GetLength())
		{
			character = row->GetAt(j);
			if (!character->IsSelected())
			{
				character->Select(TRUE);
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
				row->Next();
				pagingBuffer->Next();
			}
			else
			{
				character->Select(FALSE);
				row->Next();
				pagingBuffer->Next();
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
			j++;
		}

		if (pagingBuffer->GetSelectionBeginOffset() < 0)
		{
			pagingBuffer->MarkSelectionBegin();
		}
		note->Next();
		pagingBuffer->NextRow();
		if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
		{
			pagingBuffer->UnmarkSelectionBegin();
		}
		i++;
	}

	//4. ������ �������� �ƴ϶��, ������ �������� �̵��Ѵ�.
	if (pagingBuffer->GetEndOffset() < pagingBuffer->GetFileEndOffset())
	{
		pagingBuffer->LastRow();
		pagingBuffer->Last();
		pagingBuffer->Load();
	}

	//5. ��ũ�ѹٸ� �����Ѵ�.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

	//6. Ŭ���̾�Ʈ ������ �����Ѵ�.
	parent->Invalidate();
}