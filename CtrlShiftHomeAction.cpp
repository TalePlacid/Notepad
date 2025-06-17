#include <afxwin.h>
#include "CtrlShiftHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4966)

CtrlShiftHomeAction::CtrlShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftHomeAction::~CtrlShiftHomeAction() {

}

void CtrlShiftHomeAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* originalRow = note->GetAt(rowIndex);
	Long columnIndex = originalRow->GetCurrent();

	Long firstRowIndex = note->First();
	Glyph* row = note->GetAt(firstRowIndex);
	row->First();

	Glyph* character;
	Long j;
	Long i = 0;
	while (i < rowIndex)
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

	i = 0;
	while (i < columnIndex)
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

	this->parent->Invalidate();
}