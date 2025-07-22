#include <afxwin.h>
#include "EndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

EndAction::EndAction(CWnd* parent)
	:KeyAction(parent) {

}

EndAction::~EndAction() {

}

void EndAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	row->Last();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Last();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();
	this->parent->Invalidate();
}