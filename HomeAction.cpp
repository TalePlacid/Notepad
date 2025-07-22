#include <afxwin.h>
#include "HomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

HomeAction::HomeAction(CWnd* parent)
	:KeyAction(parent) {

}

HomeAction::~HomeAction() {

}

void HomeAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	row->First();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->First();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();
	this->parent->Invalidate();
}