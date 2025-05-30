#include <afxwin.h>
#include "CtrlShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

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

	if (columnIndex >= row->GetLength() - 1)
	{
		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			row->First();
		}
	}
	else
	{
		columnIndex = row->Next();
		Glyph* character = row->GetAt(columnIndex);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inBoundary = FALSE;
		if (!isWordCharacter)
		{
			inBoundary = TRUE;
		}

		Long i = columnIndex;
		while ((i < row->GetLength()) && !(inBoundary && isWordCharacter))
		{
			if (character->IsSelected())
			{
				character->Select(FALSE);
			}
			else
			{
				character->Select(TRUE);
			}

			columnIndex = row->Next();
			character = row->GetAt(columnIndex);
			isWordCharacter = character->IsWordCharacter();
			if (!isWordCharacter)
			{
				inBoundary = TRUE;
			}
			i++;
		}

		if (i < row->GetLength())
		{
			row->Previous();
		}
	}

	this->parent->Invalidate();
}