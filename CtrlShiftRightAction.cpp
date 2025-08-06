#include <afxwin.h>
#include "CtrlShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

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
		columnIndex = row->Next();
		pagingBuffer->Next();
		Glyph* character = row->GetAt(columnIndex - 1);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inBoundary = FALSE;
		if (!isWordCharacter)
		{
			inBoundary = TRUE;
		}

		Long i = columnIndex;
		while ((i <= row->GetLength()) && !(inBoundary && isWordCharacter))
		{
			if (!character->IsSelected())
			{
				character->Select(TRUE);
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}
				columnIndex = row->Next();
				pagingBuffer->Next();
			}
			else
			{
				character->Select(FALSE);
				columnIndex = row->Next();
				pagingBuffer->Next();
				if (markingHelper.HasReturnedToSelectionBegin())
				{
					markingHelper.Unmark();
				}
			}

			character = row->GetAt(columnIndex - 1);
			isWordCharacter = character->IsWordCharacter();
			if (!isWordCharacter)
			{
				inBoundary = TRUE;
			}
			i++;
		}

		if (i <= row->GetLength())
		{
			row->Previous();
			pagingBuffer->Previous();
		}
	}
	else
	{
		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();

			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
			}
		}
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}