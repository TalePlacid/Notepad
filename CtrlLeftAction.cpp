#include <afxwin.h>
#include "CtrlLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlLeftAction::CtrlLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlLeftAction::~CtrlLeftAction() {

}

void CtrlLeftAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	if (columnIndex > 0)
	{
		Long i = columnIndex - 1;
		Glyph* character = row->GetAt(i);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inWord = FALSE;
		if (isWordCharacter)
		{
			inWord = TRUE;
		}

		while (i >= 0 && (!inWord || isWordCharacter))
		{
			character = row->GetAt(i);
			isWordCharacter = character->IsWordCharacter();
			if (isWordCharacter)
			{
				inWord = TRUE;
			}

			row->Previous();
			i--;
		}

		if (i >= 0)
		{
			row->Next();
		}
	}
	else
	{
		if (rowIndex > 0)
		{
			rowIndex = note->Previous();
			Glyph* previousRow = note->GetAt(rowIndex);
			previousRow->Last();
		}
	}

	note->Select(false);
	this->parent->Invalidate();
}