#include <afxwin.h>
#include "CtrlRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlRightAction::CtrlRightAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlRightAction::~CtrlRightAction() {

}

void CtrlRightAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		Long i = columnIndex;
		Glyph* character = row->GetAt(i);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inBoundary = !isWordCharacter;
		while (i < row->GetLength() && (!inBoundary || !isWordCharacter))
		{
			character = row->GetAt(i);
			isWordCharacter = character->IsWordCharacter();
			if (!isWordCharacter)
			{
				inBoundary = TRUE;
			}
			row->Next();
			pagingBuffer->Next();
			i++;
		}

		if (i < row->GetLength())
		{
			row->Previous();
			pagingBuffer->Previous();
		}
	}
	else
	{
		if (note->IsBelowBottomLine(rowIndex + 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();
		}
	}

	note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

#if 0
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long columnIndex = row->GetCurrent();
	if (columnIndex < row->GetLength())
	{
		Long i = columnIndex;
		Glyph* character = row->GetAt(i);
		BOOL isWordCharacter = character->IsWordCharacter();
		BOOL inBoundary = FALSE;
		if (!isWordCharacter)
		{
			inBoundary = TRUE;
		}

		while (i < row->GetLength() && (!inBoundary || !isWordCharacter))
		{
			character = row->GetAt(i);
			isWordCharacter = character->IsWordCharacter();
			if (!isWordCharacter)
			{
				inBoundary = TRUE;
			}
			row->Next();
			pagingBuffer->Next();
			i++;
		}

		if (i < row->GetLength())
		{
			row->Previous();
			pagingBuffer->Previous();
		}
	}
	else
	{
		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
				rowIndex = note->GetCurrent();
			}
			Glyph* nextRow = note->GetAt(rowIndex);
			nextRow->First();
			pagingBuffer->First();
		}
	}

	note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
#endif
}