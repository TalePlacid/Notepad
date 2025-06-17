#include <afxwin.h>
#include "DownArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

DownArrowAction::DownArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

DownArrowAction::~DownArrowAction() {

}

void DownArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Glyph* character;
	TCHAR(*content);
	Long width = 0;

	Long i = 0;
	while (i < columnIndex)
	{
		character = row->GetAt(i);
		content = (char*)(*character);
		width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>(content));
		i++;
	}

	rowIndex = note->Next();
	Glyph* nextRow = note->GetAt(rowIndex);
	Long nextRowWidth = 0;

	i = 0;
	while (i < nextRow->GetLength() && nextRowWidth < width)
	{
		character = nextRow->GetAt(i);
		content = (char*)(*character);
		nextRowWidth += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)content));
		i++;
	}

	if (i < nextRow->GetLength() - 1)
	{
		nextRow->Move(i);
	}
	else
	{
		nextRow->Last();
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}