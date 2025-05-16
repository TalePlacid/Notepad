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
		WordState state = GetState(CString(character->MakeString().c_str()));
		BOOL inVocable = FALSE;
		if (state == VOCABLE)
		{
			inVocable = TRUE;
		}

		while (i >= 0 && (!inVocable || state != BOUNDARY))
		{
			character = row->GetAt(i);
			state = GetState(CString(character->MakeString().c_str()));
			if (state == VOCABLE)
			{
				inVocable = TRUE;
			}

			row->Previous();
			i--;
		}

		if (i >= 0)
		{
			row->Next();
		}
	}

	this->parent->Invalidate();
}