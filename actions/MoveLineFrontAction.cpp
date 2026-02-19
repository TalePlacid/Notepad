#include <afxwin.h>
#include "MoveLineFrontAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

MoveLineFrontAction::MoveLineFrontAction(CWnd* parent)
	:Action(parent) {

}

MoveLineFrontAction::~MoveLineFrontAction() {

}

void MoveLineFrontAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 노트에서 이동한다.
	Long firstColumnIndex = row->First();

	//3. 페이징 버퍼에서 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Previous(columnIndex - firstColumnIndex);
}