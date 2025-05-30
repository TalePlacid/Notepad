#include <afxwin.h>
#include "CtrlShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlShiftLeftAction::CtrlShiftLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftLeftAction::~CtrlShiftLeftAction() {

}

void CtrlShiftLeftAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	if (columnIndex < 0)
	{
		if (rowIndex > 0)
		{
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			row->Last();
		}
	}
	else
	{
		Glyph* character = row->GetAt(columnIndex);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inWord = FALSE;
		if (isWordCharacter)
		{
			inWord = TRUE;
		}

		while ((columnIndex > -1) && (!inWord || isWordCharacter))
		{
			character = row->GetAt(columnIndex);
			isWordCharacter = character->IsWordCharacter();
			if (isWordCharacter)
			{
				inWord = TRUE;
			}

			if (character->IsSelected())
			{
				character->Select(FALSE);
			}
			else
			{
				character->Select(TRUE);
			}

			columnIndex = row->Previous();
		}

		if (columnIndex > -1)
		{
			columnIndex = row->Next();
			character = row->GetAt(columnIndex);
			if (character->IsSelected())
			{
				character->Select(FALSE);
			}
			else
			{
				character->Select(TRUE);
			}
		}
	}
	
	this->parent->Invalidate();
}