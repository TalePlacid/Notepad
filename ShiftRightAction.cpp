#include <afxwin.h>
#include "ShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ShiftRightAction::ShiftRightAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftRightAction::~ShiftRightAction() {

}

void ShiftRightAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		Glyph* character = row->GetAt(columnIndex);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
		}
		else
		{
			character->Select(FALSE);
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnMarkSelectionBegin();
			}
		}

		row->Next();
		pagingBuffer->Next();
	}
	else
	{
		rowIndex = note->Next();
		pagingBuffer->NextRow();
		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
			note = ((NotepadForm*)(this->parent))->note;
			rowIndex = note->GetCurrent();
		}

		row = note->GetAt(rowIndex);
		row->First();
		pagingBuffer->First();
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}