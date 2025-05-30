#include <afxwin.h>
#include "CtrlShiftEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlShiftEndAction::CtrlShiftEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftEndAction::~CtrlShiftEndAction() {

}

void CtrlShiftEndAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* originalRow = note->GetAt(rowIndex);
	Long columnIndex = originalRow->GetCurrent();

	Long lastRowIndex = note->Last();
	Glyph* row = note->GetAt(lastRowIndex);
	row->Last();

	Glyph* character;
	Long i = columnIndex + 1;
	while (i < originalRow->GetLength())
	{
		character = originalRow->GetAt(i);
		if (character->IsSelected())
		{
			character->Select(FALSE);
		}
		else
		{
			character->Select(TRUE);
		}
		i++;
	}

	Long j;
	i = rowIndex + 1;
	while (i <= lastRowIndex)
	{
		row = note->GetAt(i);
		j = 0;
		while (j < row->GetLength())
		{
			character = row->GetAt(j);
			if (character->IsSelected())
			{
				character->Select(FALSE);
			}
			else
			{
				character->Select(TRUE);
			}
			j++;
		}
		i++;
	}

	this->parent->Invalidate();
}