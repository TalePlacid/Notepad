#include <afxwin.h>
#include "CtrlRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlRightAction::CtrlRightAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlRightAction::~CtrlRightAction() {

}

void CtrlRightAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	if (columnIndex < row->GetLength())
	{
		Long i = columnIndex;
		Glyph* character = row->GetAt(i);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inBoundary = FALSE;
		if (!isWordCharacter)
		{
			inBoundary = TRUE;
		}

		while (i < row->GetLength() && (!inBoundary || !isWordCharacter))
		{
			character = row->GetAt(i);
			isWordCharacter = character->IsWordCharacter();
			if (!isWordCharacter)
			{
				inBoundary = TRUE;
			}
			row->Next();
			i++;
		}

		if (i < row->GetLength())
		{
			row->Previous();
		}
	}
	else
	{
		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			Glyph* nextRow = note->GetAt(rowIndex);
			nextRow->First();
		}
	}

	note->Select(false);
	this->parent->Invalidate();
}