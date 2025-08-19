#include <afxwin.h>
#include "MarkingHelper.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

MarkingHelper::MarkingHelper(CWnd* parent) {
	this->parent = parent;
}

MarkingHelper::~MarkingHelper() {

}

void MarkingHelper::Mark() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() < 0)
	{
		pagingBuffer->MarkSelectionBegin();
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	note->Mark(rowIndex);

	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	row->Mark(columnIndex);
}

void MarkingHelper::Unmark() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->UnmarkSelectionBegin();

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Unmark();
}

bool MarkingHelper::IsUnmarked() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	return pagingBuffer->GetSelectionBeginOffset() < 0;
}

bool MarkingHelper::HasReturnedToSelectionBegin() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	return pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset();
}