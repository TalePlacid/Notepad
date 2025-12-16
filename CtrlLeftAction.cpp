#include <afxwin.h>
#include "CtrlLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlLeftAction::CtrlLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlLeftAction::~CtrlLeftAction() {

}

void CtrlLeftAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		Long i = columnIndex - 1;
		Glyph* character = row->GetAt(i);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inWord = isWordCharacter;

		while (i >= 0 && (!inWord || isWordCharacter))
		{
			character = row->GetAt(i);
			isWordCharacter = character->IsWordCharacter();
			if (isWordCharacter)
			{
				inWord = TRUE;
			}

			row->Previous();
			pagingBuffer->Previous();
			i--;
		}

		if (i >= 0)
		{
			row->Next();
			pagingBuffer->Next();
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
			rowIndex = note->Previous();
			pagingBuffer->PreviousRow();
			row = note->GetAt(rowIndex);
			row->Last();
			pagingBuffer->Last();
		}
	}
}