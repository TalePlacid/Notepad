#include <afxwin.h>
#include "HScrollBarPageLeftAction.h"
#include "Glyph.h"
#include "NotepadForm.h"
#include "Caret.h"
#include "CaretController.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

HScrollBarPageLeftAction::HScrollBarPageLeftAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarPageLeftAction::~HScrollBarPageLeftAction() {

}

void HScrollBarPageLeftAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS | SIF_PAGE;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	scrollInfo.nPos -= scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaWidth = rect.right - rect.left;

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long x = caret->GetX() + scrollInfo.nPage;

	if (x > clientAreaWidth)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
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