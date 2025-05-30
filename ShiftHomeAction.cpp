#include <afxwin.h>
#include "ShiftHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

ShiftHomeAction::ShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftHomeAction::~ShiftHomeAction() {

}

void ShiftHomeAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	row->First();

	Glyph* character;
	Long i = 0;
	while (i <= columnIndex)
	{
		character = row->GetAt(i);
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