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
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	
	if (columnIndex > 0)
	{
		row->Previous();
	}
	else
	{
		if (rowIndex > 0)
		{
			rowIndex = note->Previous();
			Glyph* previousRow = note->GetAt(rowIndex);
			previousRow->Last();
		}
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}