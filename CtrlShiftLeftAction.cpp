#include <afxwin.h>
#include "CtrlShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

CtrlShiftLeftAction::CtrlShiftLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftLeftAction::~CtrlShiftLeftAction() {

}

void CtrlShiftLeftAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		Glyph* character = row->GetAt(columnIndex - 1);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inWord = FALSE;
		if (isWordCharacter)
		{
			inWord = TRUE;
		}

		while ((columnIndex > 0) && (!inWord || isWordCharacter))
		{
			character = row->GetAt(columnIndex - 1);
			isWordCharacter = character->IsWordCharacter();
			if (isWordCharacter)
			{
				inWord = TRUE;
			}

			if (!character->IsSelected())
			{
				character->Select(TRUE);
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
				columnIndex = row->Previous();
				pagingBuffer->Previous();
			}
			else
			{
				character->Select(FALSE);
				columnIndex = row->Previous();
				pagingBuffer->Previous();
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
		}

		if (columnIndex > 0)
		{
			columnIndex = row->Next();
			pagingBuffer->Next();
			character = row->GetAt(columnIndex - 1);
			if (!character->IsSelected())
			{
				character->Select(TRUE);
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
			}
			else
			{
				character->Select(FALSE);
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
		}
	}
	else
	{
		if (rowIndex > 0)
		{
			rowIndex = note->Previous();
			pagingBuffer->PreviousRow();
			row = note->GetAt(rowIndex);
			row->Last();
			pagingBuffer->Last();

			if (!pagingBuffer->IsBelowTopLine())
			{
				pagingBuffer->Load();
			}
		}
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}