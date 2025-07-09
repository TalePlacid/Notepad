#include <afxwin.h>
#include "CtrlEndAction_Fixed.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

CtrlEndAction_Fixed::CtrlEndAction_Fixed(CWnd* parent)
	:KeyAction(parent) {

}

CtrlEndAction_Fixed::~CtrlEndAction_Fixed() {

}

void CtrlEndAction_Fixed::Perform() {
	//1. ����¡ ���ۿ��� ������ ��ġ�� �̵��Ѵ�.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->LastRow();
	pagingBuffer->Last();
	//2. ����¡ ������ ���� �������� ������ ������ �������� �ƴϸ�,
	if (pagingBuffer->GetEndOffset() < pagingBuffer->GetFileEnd())
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

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}