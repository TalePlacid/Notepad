#include <afxwin.h>
#include "ShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

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

	Glyph* character = NULL;
	if (columnIndex > -1)
	{
		character = row->GetAt(columnIndex);
	}

	if (columnIndex < row->GetLength() - 1)
	{
		columnIndex = row->Next();
	}
	else
	{
		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}
	}

	Glyph* movedCharacter = NULL;
	if (columnIndex > -1)
	{
		movedCharacter = row->GetAt(columnIndex);
	}

	if ((movedCharacter != NULL) && (character != movedCharacter))
	{
		if (movedCharacter->IsSelected())
		{
			movedCharacter->Select(FALSE);
		}
		else
		{
			movedCharacter->Select(TRUE);
		}
	}

	this->parent->Invalidate();
}