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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Glyph* character;
	CString content;
	Long width = 0;

	Long i = 0;
	while (i <= columnIndex)
	{
		character = row->GetAt(i);
		content = CString(character->MakeString().c_str());
		width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)content));
		i++;
	}

	rowIndex = ((NotepadForm*)(this->parent))->note->Next();
	Glyph* nextRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
	Long nextRowWidth = 0;

	i = 0;
	while (i < nextRow->GetLength() && nextRowWidth < width)
	{
		character = nextRow->GetAt(i);
		content = CString(character->MakeString().c_str());
		nextRowWidth += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)content));
		i++;
	}
	i--;

	if (i < nextRow->GetLength() - 1)
	{
		nextRow->Move(i);
	}
	else
	{
		nextRow->Last();
	}

	this->parent->Invalidate();
}