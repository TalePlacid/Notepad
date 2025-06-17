#include <afxwin.h>
#include "ShiftEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

ShiftEndAction::ShiftEndAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftEndAction::~ShiftEndAction() {

}

void ShiftEndAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	Long lastIndex = row->Last();

	Glyph* character;
	Long i = columnIndex;
	while (i < lastIndex)
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