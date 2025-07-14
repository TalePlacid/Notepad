#include <afxwin.h>
#include "ShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ShiftLeftAction::ShiftLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftLeftAction::~ShiftLeftAction() {

}

void ShiftLeftAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		Glyph* character = row->GetAt(columnIndex - 1);

		row->Previous();
		pagingBuffer->Previous();
		if (character->IsSelected())
		{
			character->Select(FALSE);
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
		else
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
		}
	}
	else if (rowIndex > 0)
	{
		rowIndex = note->Previous();
		pagingBuffer->PreviousRow();
		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
			note = ((NotepadForm*)(this->parent))->note;
			rowIndex = note->GetCurrent();
		}
		row = note->GetAt(rowIndex);
		row->Last();
		pagingBuffer->Last();
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}