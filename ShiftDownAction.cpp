#include <afxwin.h>
#include "ShiftDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

ShiftDownAction::ShiftDownAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftDownAction::~ShiftDownAction() {

}

void ShiftDownAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	if (rowIndex < note->GetLength() - 1)
	{
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Long originalWidth = 0;
		Glyph* character;
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

		Long nextRowIndex = note->Next();
		Glyph* nextRow = note->GetAt(nextRowIndex);

		Long width = 0;
		i = 0;
		while ((i < nextRow->GetLength())&&(width < originalWidth))
		{
			character = nextRow->GetAt(i);
			content = (char*)(*character);
			width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(
				const_cast<char*>(content));
			i++;
		}

		Long nextColumnIndex;
		if (i < nextRow->GetLength())
		{
			nextColumnIndex = nextRow->Move(i);
		}
		else
		{
			nextColumnIndex = nextRow->Last();
		}

		i = columnIndex;
		while (i < row->GetLength())
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

		i = 0;
		while (i < nextColumnIndex)
		{
			character = nextRow->GetAt(i);
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