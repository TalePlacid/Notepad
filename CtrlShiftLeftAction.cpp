#include <afxwin.h>
#include "CtrlShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "resource.h"

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

	MarkingHelper markingHelper(this->parent);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		Glyph* character = row->GetAt(columnIndex - 1);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inWord = isWordCharacter;
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
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}
			}
			else
			{
				character->Select(FALSE);
			}
			columnIndex = row->Previous();
			pagingBuffer->Previous();
			if (markingHelper.HasReturnedToSelectionBegin())
			{
				markingHelper.Unmark();
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
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}
			}
			else
			{
				character->Select(FALSE);
				if (markingHelper.HasReturnedToSelectionBegin())
				{
					markingHelper.Unmark();
				}
			}
		}
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex + 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex > 0)
		{
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			row->Last();
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
		}
	}
}