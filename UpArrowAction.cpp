#include <afxwin.h>
#include "UpArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

UpArrowAction::UpArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

UpArrowAction::~UpArrowAction() {

}

void UpArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Glyph* character;
	Long width = 0;
	TCHAR(*content);

	Long i = 0;
	while (i < columnIndex)
	{
		character = row->GetAt(i);
		content = (char*)(*character);
		width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>(content));
		i++;
	}

	rowIndex = note->Previous();
	Glyph* previousRow = note->GetAt(rowIndex);
	Long previousRowWidth = 0;
	i = 0;
	while (i < previousRow->GetLength() && previousRowWidth < width)
	{
		character = previousRow->GetAt(i);
		content = (char*)(*character);
		previousRowWidth += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>(content));
		i++;
	}

	if (i < previousRow->GetLength() - 1)
	{
		previousRow->Move(i);
	}
	else
	{
		previousRow->Last();
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}