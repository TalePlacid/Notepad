#include <afxwin.h>
#include "CtrlShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlShiftRightAction::CtrlShiftRightAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftRightAction::~CtrlShiftRightAction() {

}

void CtrlShiftRightAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		Glyph* character = row->GetAt(columnIndex);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inBoundary = !isWordCharacter;
		while (columnIndex < row->GetLength() && !(inBoundary && isWordCharacter))
		{
			character = row->GetAt(columnIndex);
			isWordCharacter = character->IsWordCharacter();
			if (!isWordCharacter)
			{
				inBoundary = TRUE;
			}

			if (!character->IsSelected())
			{
				character->Select(true);
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
			}
			else
			{
				character->Select(false);
			}

			columnIndex = row->Next();
			pagingBuffer->Next();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}

		if (inBoundary && isWordCharacter)
		{
			columnIndex = row->Previous();
			character = row->GetAt(columnIndex);
			if (!character->IsSelected())
			{
				character->Select(true);
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
			}
			else
			{
				character->Select(false);
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
			
			pagingBuffer->Previous();
		}
	}
	else
	{
		if (note->IsBelowBottomLine(rowIndex + 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();
		}
	}
}