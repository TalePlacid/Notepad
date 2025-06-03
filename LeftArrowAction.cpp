#include <afxwin.h>
#include "LeftArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

LeftArrowAction::LeftArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

LeftArrowAction::~LeftArrowAction() {

}

void LeftArrowAction::Perform() {
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	row->Previous();

	if (columnIndex <= -1 && rowIndex > 0)
	{
		rowIndex = ((NotepadForm*)(this->parent))->note->Previous();
		Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
		previousRow->Last();
	}

	((NotepadForm*)(this->parent))->Notify("Unselect");
	this->parent->Invalidate();
}