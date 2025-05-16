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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	Long i = columnIndex + 1;
	if (i >= row->GetLength())
	{
		if (rowIndex < ((NotepadForm*)(this->parent))->note->GetLength() - 1)
		{
			rowIndex = ((NotepadForm*)(this->parent))->note->Next();
			Glyph* nextRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
			nextRow->First();
		}
	}
	else
	{
		Glyph* character = row->GetAt(i);
		CString content = CString(character->MakeString().c_str());
		WordState state = GetState(content);
		BOOL inBoundary = FALSE;
		if (state == BOUNDARY)
		{
			inBoundary = TRUE;
		}

		while (i < row->GetLength() && (!inBoundary || state != VOCABLE))
		{
			character = row->GetAt(i);
			content = CString(character->MakeString().c_str());
			state = GetState(content);
			if (state == BOUNDARY)
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

	this->parent->Invalidate();
}