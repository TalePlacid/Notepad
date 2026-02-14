#include <afxwin.h>
#include "EndAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

EndAction::EndAction(CWnd* parent)
	:KeyAction(parent) {

}

EndAction::~EndAction() {

}

void EndAction::Perform() {
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