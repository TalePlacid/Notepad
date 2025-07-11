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
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
	row->First();
	((NotepadForm*)(this->parent))->pagingBuffer->First();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}