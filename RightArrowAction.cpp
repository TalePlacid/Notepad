#include <afxwin.h>
#include "RightArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

RightArrowAction::RightArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

RightArrowAction::~RightArrowAction() {

}

void RightArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	if (row->GetCurrent() < row->GetLength())
	{
		row->Next();
	}
	else
	{
		if (note->GetCurrent() < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			Glyph* nextRow = note->GetAt(rowIndex);
			nextRow->First();
		}
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}