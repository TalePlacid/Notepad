#include <afxwin.h>
#include "HScrollBarLeftClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Caret.h"
#include "CaretController.h"
#include "Glyph.h"

#pragma warning(disable:4996)

HScrollBarLeftClickAction::HScrollBarLeftClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarLeftClickAction::~HScrollBarLeftClickAction() {

}

void HScrollBarLeftClickAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	scrollInfo.nPos -= sizeCalculator->GetMultiByteWidth();
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaWidth = rect.right - rect.left;

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long x = caret->GetX() + sizeCalculator->GetMultiByteWidth();

	if (x > clientAreaWidth)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long width = 0;
		Long i = 0;
		if (x > clientAreaWidth)
		{
			while (i < row->GetLength() && width < scrollInfo.nPos + clientAreaWidth)
			{
				character = row->GetAt(i);
				width += sizeCalculator->GetCharacterWidth((char*)(*character));
				i++;
			}
		}

		if (i < row->GetLength())
		{
			i--;
		}

		row->Move(i);
	}

	this->parent->Invalidate();
}