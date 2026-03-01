#include <afxwin.h>
#include "MoveLineRearAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

MoveLineRearAction::MoveLineRearAction(CWnd* parent)
	:Action(parent) {

}

MoveLineRearAction::~MoveLineRearAction() {

}

void MoveLineRearAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 노트에서 이동한다.
	Long lastColumnIndex = row->Last();

	//3. 페이징 버퍼에서 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Next(lastColumnIndex - columnIndex);
}
