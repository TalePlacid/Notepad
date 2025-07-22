#include <afxwin.h>
#include "ShiftHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

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

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex - 1;
	while (i >= 0)
	{
		character = row->GetAt(i);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			row->Previous();
			pagingBuffer->Previous();
		}
		else
		{
			character->Select(FALSE);
			row->Previous();
			pagingBuffer->Previous();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
		i--;
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}