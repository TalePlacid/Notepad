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
	//1. ����¡ ���ۿ��� ������ ��ġ�� �̵��Ѵ�.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->LastRow();
	pagingBuffer->Last();
	//2. ����¡ ������ ���� �������� ������ ������ �������� �ƴϸ�,
	if (pagingBuffer->GetEndOffset() < pagingBuffer->GetFileEndOffset())
	{
		//1.1. ����¡ ���ۿ��� ������ �������� �����Ѵ�.
		pagingBuffer->Load();
	}
	else //2. ������ �������̸�,
	{
		//2.1. ��Ʈ���� ������ ��ġ�� �̵��Ѵ�.
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