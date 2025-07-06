#include <afxwin.h>
#include "HScrollBarDragAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "Caret.h"
#include "CaretController.h"
#include "ScrollBarController.h"

#pragma warning(disable:4996)

HScrollBarDragAction::HScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	this->nPos = nPos;
}

HScrollBarDragAction::~HScrollBarDragAction() {

}

void HScrollBarDragAction::Perform() {
	//1. 드래그 위치로 스크롤바의 현재 위치를 설정한다.
	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, this->nPos, TRUE);

	//2. 화면 너비를 구한다.
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaWidth = rect.right - rect.left;
	
	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();

	if (caret->GetX() < 0 || caret->GetX() > clientAreaWidth)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long widthLimit = 0;
		if (caret->GetX() < 0)
		{
			widthLimit = this->nPos;
		}
		else if (caret->GetX() > clientAreaWidth)
		{
			widthLimit = this->nPos + clientAreaWidth;
		}

		Glyph* character;
		Long width = 0;
		Long i = 0;
		while (i < row->GetLength() && width <= widthLimit)
		{
			character = row->GetAt(i);
			width += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		row->Move(i - 1);
	}

	this->parent->Invalidate();
}