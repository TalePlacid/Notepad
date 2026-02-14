#include <afxwin.h>
#include "ShiftEndAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

ShiftEndAction::ShiftEndAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftEndAction::~ShiftEndAction() {

}

void ShiftEndAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 끝까지 반복한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (columnIndex < row->GetLength())
	{
		row->GetAt(columnIndex)->ToggleSelection();
		columnIndex = row->Next();

		pagingBuffer->BeginSelectionIfNeeded();
		pagingBuffer->Next();
		pagingBuffer->EndSelectionIfCollapsed();
	}
}