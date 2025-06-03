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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	Long i = columnIndex;
	if (i < 0)
	{
		if (rowIndex > 0)
		{
			rowIndex = ((NotepadForm*)(this->parent))->note->Previous();
			Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
			previousRow->Last();
		}
	}
	else
	{
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

	((NotepadForm*)(this->parent))->Notify("Unselect");
	this->parent->Invalidate();
}