#include <afxwin.h>
#include "ShiftEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ShiftEndAction::ShiftEndAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftEndAction::~ShiftEndAction() {

}

void ShiftEndAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* character;
	Long i = columnIndex;
	while (i < row->GetLength())
	{
		character = row->GetAt(i);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			row->Next();
			pagingBuffer->Next();
		}
		else
		{
			character->Select(FALSE);
			row->Next();
			pagingBuffer->Next();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
		i++;
	}
}