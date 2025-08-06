#include <afxwin.h>
#include "CtrlEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

CtrlEndAction::CtrlEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlEndAction::~CtrlEndAction() {

}

void CtrlEndAction::Perform() {
	//1. 페이징 버퍼에서 마지막 위치로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->LastRow();
	pagingBuffer->Last();
	//2. 페이징 버퍼의 현재 페이지가 파일의 마지막 페이지가 아니면,
	if (pagingBuffer->GetEndOffset() < pagingBuffer->GetFileEndOffset())
	{
		//1.1. 페이징 버퍼에서 마지막 페이지를 적재한다.
		pagingBuffer->Load();
	}
	else //2. 마지막 페이지이면,
	{
		//2.1. 노트에서 마지막 위치로 이동한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->Last();
		Glyph* row = note->GetAt(rowIndex);
		row->Last();
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}