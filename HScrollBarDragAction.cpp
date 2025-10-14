#include <afxwin.h>
#include "HScrollBarDragAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "Caret.h"
#include "CaretController.h"
#include "scrollBarAdapter.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

HScrollBarDragAction::HScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	this->nPos = nPos;
}

HScrollBarDragAction::~HScrollBarDragAction() {

}

void HScrollBarDragAction::Perform() {
	//1. ���� ��ũ�ѹ� ��ġ�� �д´�.
	Long originalPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);

	//2. �巡�� ��ġ�� ��ũ�ѹ��� ���� ��ġ�� �����Ѵ�.
	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, this->nPos, TRUE);

	//3. ȭ�� �ʺ� ���Ѵ�.
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaWidth = rect.right - rect.left;
	
	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long x = caret->GetX() + (originalPos - this->nPos);

	if (x < 0 || x > clientAreaWidth)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		
		Glyph* character;
		Long width = 0;
		Long i = 0;
		if (x < 0)
		{
			while (i < row->GetLength() && width <= this->nPos)
			{
				character = row->GetAt(i);
				width += sizeCalculator->GetCharacterWidth((char*)(*character));
				i++;
			}
		}
		else if (x > clientAreaWidth)
		{
			while (i < row->GetLength() && width < this->nPos + clientAreaWidth)
			{
				character = row->GetAt(i);
				width += sizeCalculator->GetCharacterWidth((char*)(*character));
				i++;
			}

			if (i < row->GetLength())
			{
				i--;
			}
		}

		row->Move(i);
		((NotepadForm*)(this->parent))->pagingBuffer->Move(i);
	}

	this->parent->Invalidate();
}