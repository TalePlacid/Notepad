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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	if (rowIndex > 0)
	{
		Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long width = 0;
		CString content;

		Long i = 0;
		while (i <= columnIndex)
		{
			character = row->GetAt(i);
			content = CString(character->MakeString().c_str());
			width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)content));
			i++;
		}

		rowIndex = ((NotepadForm*)(this->parent))->note->Previous();
		Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
		Long previousRowWidth = 0;
		i = 0;
		while (i < previousRow->GetLength() && previousRowWidth < width)
		{
			character = previousRow->GetAt(i);
			content = CString(character->MakeString().c_str());
			previousRowWidth += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)content));
			i++;
		}
		i--;

		if (i < previousRow->GetLength() - 1)
		{
			previousRow->Move(i);
		}
		else
		{
			previousRow->Last();
		}

		((NotepadForm*)(this->parent))->Notify("Unselect");
		this->parent->Invalidate();
	}
}