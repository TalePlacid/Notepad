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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
	row->Last();
	((NotepadForm*)(this->parent))->pagingBuffer->Last();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}