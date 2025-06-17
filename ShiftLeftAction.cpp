#include <afxwin.h>
#include "ShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

ShiftLeftAction::ShiftLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftLeftAction::~ShiftLeftAction() {

}

void ShiftLeftAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph *row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	
	Glyph* character = NULL;
	if (columnIndex > 0)
	{
		character = row->GetAt(columnIndex - 1);
	}

	Long previousIndex = columnIndex;
	columnIndex = row->Previous();
	if (columnIndex == previousIndex && rowIndex > 0)
	{
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();
	}

	if (character != NULL)
	{
		if (character->IsSelected())
		{
			character->Select(FALSE);
		}
		else
		{
			character->Select(TRUE);
		}
	}

	this->parent->Invalidate();
}