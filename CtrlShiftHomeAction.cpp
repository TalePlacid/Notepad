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
	//1. ��Ʈ���� ���� ��ġ�� �д´�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. �ٿ��� ���� ĭ���� ���� ĭ���� �ݺ��Ѵ�.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex - 1;
	while (i >= 0)
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

	//3. ��Ʈ���� ó�� ��ġ���� �ݺ��Ѵ�.
	Long j;
	i = rowIndex;
	while (i >= 0)
	{
		//3.1. ������ ���ÿ��ο� ���� �����Ѵ�.
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

	//4. ù �������� �ƴϸ�, ù �������� �����Ѵ�.
	if (pagingBuffer->GetStartOffset() > 0)
	{
		pagingBuffer->FirstRow();
		pagingBuffer->Load();
	}

	//5. ��ũ�ѹٸ� �����Ѵ�.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

	//6. Ŭ���̾�Ʈ ������ �����Ѵ�.
	this->parent->Invalidate();
}