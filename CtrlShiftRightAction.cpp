#include <afxwin.h>
#include "CtrlShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
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

	MarkingHelper markingHelper(this->parent);
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
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}
			}
			else
			{
				character->Select(false);
			}

			columnIndex = row->Next();
			pagingBuffer->Next();
			if (markingHelper.HasReturnedToSelectionBegin())
			{
				markingHelper.Unmark();
			}
		}

		if (inBoundary && isWordCharacter)
		{
			columnIndex = row->Previous();
			character = row->GetAt(columnIndex);
			if (!character->IsSelected())
			{
				character->Select(true);
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}
			}
			else
			{
				character->Select(false);
				if (markingHelper.HasReturnedToSelectionBegin())
				{
					markingHelper.Unmark();
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