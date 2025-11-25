#include <afxwin.h>
#include "CtrlHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

CtrlHomeAction::CtrlHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlHomeAction::~CtrlHomeAction() {

}

void CtrlHomeAction::Perform() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->FirstRow();

	Glyph* loadedNote = pagingBuffer->LoadNext();
	
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(-rowStartIndex);

	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	((NotepadForm*)(this->parent))->note = loadedNote;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->First();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->First();
#if 0
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

	note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
#endif
}