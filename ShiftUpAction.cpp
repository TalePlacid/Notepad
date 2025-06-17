#include <afxwin.h>
#include "ShiftUpAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

ShiftUpAction::ShiftUpAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftUpAction::~ShiftUpAction() {

}

void ShiftUpAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	if (rowIndex > 0)
	{
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;

		Long originalWidth = 0;
		TCHAR(*content);
		Long i = 0;
		while (i < columnIndex)
		{
			character = row->GetAt(i);
			content = (char*)(*character);
			originalWidth += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(
				const_cast<char*>(content));
			i++;
		}

		Long previousRowIndex = note->Previous();
		Glyph* previousRow = note->GetAt(previousRowIndex);

		Long width = 0;
		i = 0;
		while ((i < previousRow->GetLength()) && (width < originalWidth))
		{
			character = previousRow->GetAt(i);
			content = (char*)(*character);
			width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(
				const_cast<char*>(content));
			i++;
		}

		Long previousColumnIndex;
		if (i < previousRow->GetLength())
		{
			previousColumnIndex = previousRow->Move(i);
		}
		else
		{
			previousColumnIndex = previousRow->Last();
		}

		i = previousColumnIndex;
		while (i < previousRow->GetLength())
		{
			character = previousRow->GetAt(i);
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

		i = 0;
		while (i < columnIndex)
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
}