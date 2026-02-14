#include <afxwin.h>
#include "CtrlShiftLeftAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

CtrlShiftLeftAction::CtrlShiftLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftLeftAction::~CtrlShiftLeftAction() {

}

void CtrlShiftLeftAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 처음이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		//2.1. 노트에서 이동한다.
		Long wordStart = row->FindPreviousWordStart(columnIndex);
		Long moved = row->Move(wordStart);

		//2.2. 노트에서 선택한다.
		row->SelectRange(moved, columnIndex);

		//2.3. 페이징버퍼에서 선택하며 이동한다.
		Long count = columnIndex - moved;
		Long i = 0;
		while (i < count)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
			i++;
		}
	}
	else //3. 줄의 처음이면,
	{
		//3.1. 적재범위를 벗어났으면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		//3.2. 첫번째 줄이 아니면,
		if (rowIndex > 0)
		{
			//3.2.1. 노트에서 이동한다.
			row = note->GetAt(rowIndex);
			rowIndex = note->Previous();
			Glyph* movedRow = note->GetAt(rowIndex);
			columnIndex = movedRow->Last();

			//3.2.2. 원래줄이 진짜줄이면, 페이징버퍼에서 선택하며 이동한다.
			if (!row->IsDummyRow())
			{
				pagingBuffer->BeginSelectionIfNeeded();
				pagingBuffer->PreviousRow();
				pagingBuffer->Last();
				pagingBuffer->EndSelectionIfCollapsed();
			}
		}
	}
}