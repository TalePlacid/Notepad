#include <afxwin.h>
#include "ShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftRightAction::ShiftRightAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftRightAction::~ShiftRightAction() {

}

void ShiftRightAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	MarkingHelper markingHelper(this->parent);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		Glyph* character = row->GetAt(columnIndex);
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
	}
	else
	{
		if (note->IsBelowBottomLine(rowIndex+1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex < note->GetLength() - 1)
		{
			if (markingHelper.IsUnmarked())
			{
				markingHelper.Mark();
			}

			rowIndex = note->Next();
			pagingBuffer->NextRow();

			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();

			if (markingHelper.HasReturnedToSelectionBegin())
			{
				markingHelper.Unmark();
			}
		}
	}
}