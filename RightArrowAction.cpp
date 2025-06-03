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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	if (row->GetCurrent() < row->GetLength() - 1)
	{
		row->Next();
	}
	else
	{
		if (((NotepadForm*)(this->parent))->note->GetCurrent() < ((NotepadForm*)(this->parent))->note->GetLength() - 1)
		{
			rowIndex = ((NotepadForm*)(this->parent))->note->Next();
			Glyph* nextRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
			nextRow->First();
		}
	}

	((NotepadForm*)(this->parent))->Notify("Unselect");
	this->parent->Invalidate();
}