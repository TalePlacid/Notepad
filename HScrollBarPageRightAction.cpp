#include <afxwin.h>
#include "HScrollBarPageRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "Caret.h"
#include "CaretController.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

HScrollBarPageRightAction::HScrollBarPageRightAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarPageRightAction::~HScrollBarPageRightAction() {

}

void HScrollBarPageRightAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS | SIF_PAGE;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	scrollInfo.nPos += scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long x = caret->GetX() - scrollInfo.nPage;

	if (x < 0)
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