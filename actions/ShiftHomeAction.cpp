#include <afxwin.h>
#include "ShiftHomeAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

ShiftHomeAction::ShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftHomeAction::~ShiftHomeAction() {

}

void ShiftHomeAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄에서 앞까지 반복한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (columnIndex > 0)
	{
		columnIndex = row->Previous();
		row->GetAt(columnIndex)->ToggleSelection();

		pagingBuffer->BeginSelectionIfNeeded();
		pagingBuffer->Previous();
		pagingBuffer->EndSelectionIfCollapsed();
	}
}