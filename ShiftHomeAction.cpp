#include <afxwin.h>
#include "ShiftHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

ShiftHomeAction::ShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftHomeAction::~ShiftHomeAction() {

}

void ShiftHomeAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	MarkingHelper markingHelper(this->parent);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex - 1;
	while (i >= 0)
	{
		character = row->GetAt(i);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (markingHelper.IsUnmarked())
			{
				markingHelper.Mark();
			}
			row->Previous();
			pagingBuffer->Previous();
		}
		else
		{
			character->Select(FALSE);
			row->Previous();
			pagingBuffer->Previous();
			if (markingHelper.HasReturnedToSelectionBegin())
			{
				markingHelper.Unmark();
			}
		}
		i--;
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}