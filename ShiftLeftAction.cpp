#include <afxwin.h>
#include "ShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"

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
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex > 0)
		{
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}

			rowIndex = note->Previous();
			pagingBuffer->PreviousRow();
			row = note->GetAt(rowIndex);
			row->Last();
			pagingBuffer->Last();

			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
	}
}