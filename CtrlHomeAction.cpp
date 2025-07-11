#include <afxwin.h>
#include "CtrlHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

CtrlHomeAction::CtrlHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlHomeAction::~CtrlHomeAction() {

}

void CtrlHomeAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->FirstRow();
	if (!pagingBuffer->IsBelowTopLine())
	{
		pagingBuffer->Load();
		note = ((NotepadForm*)(this->parent))->note;
	}
	else
	{
		Long rowIndex = note->First();
		Glyph* row = note->GetAt(rowIndex);
		row->First();
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	note->Select(false);
	this->parent->Invalidate();
}