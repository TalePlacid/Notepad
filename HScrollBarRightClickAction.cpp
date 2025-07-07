#include <afxwin.h>
#include "HScrollBarRightClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Glyph.h"
#include "Caret.h"
#include "CaretController.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

HScrollBarRightClickAction::HScrollBarRightClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarRightClickAction::~HScrollBarRightClickAction() {

}

void HScrollBarRightClickAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	scrollInfo.nPos += sizeCalculator->GetMultiByteWidth();
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long x = caret->GetX() - sizeCalculator->GetMultiByteWidth();

	if (x < 0)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long width = 0;
		Long i = 0;
		if (x < 0)
		{
			while (i < row->GetLength() && width <= scrollInfo.nPos)
			{
				character = row->GetAt(i);
				width += sizeCalculator->GetCharacterWidth((char*)(*character));
				i++;
			}
		}

		row->Move(i);
		((NotepadForm*)(this->parent))->pagingBuffer->Move(i);
	}

	this->parent->Invalidate();
}