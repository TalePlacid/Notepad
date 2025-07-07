#include <afxwin.h>
#include "VScrollBarPageUpAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "CaretController.h"
#include "Caret.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

VScrollBarPageUpAction::VScrollBarPageUpAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageUpAction::~VScrollBarPageUpAction() {

}

void VScrollBarPageUpAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	scrollInfo.nPos -= scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long y = caret->GetY() + scrollInfo.nPage;

	if (y > clientAreaHeight)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* originalRow = note->GetAt(rowIndex);
		Long columnIndex = originalRow->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Glyph* character;
		Long originalWidth = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			character = originalRow->GetAt(i);
			originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		i = rowIndex;
		while (i >= 0 && y > clientAreaHeight - sizeCalculator->GetRowHeight())
		{
			rowIndex = note->Previous();
			pagingBuffer->PreviousRow();
			if (!pagingBuffer->IsBelowTopLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
				rowIndex = note->GetCurrent();
			}
			y -= sizeCalculator->GetRowHeight();
			i--;
		}

		Glyph* row = note->GetAt(rowIndex);
		Long previousWidth = 0;
		Long width = 0;
		i = 0;
		while (i < row->GetLength() && width < originalWidth)
		{
			character = row->GetAt(i);
			previousWidth = width;
			width += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		if (width - originalWidth > originalWidth - previousWidth)
		{
			i--;
		}

		row->Move(i);
		pagingBuffer->Move(i);
	}

	this->parent->Invalidate();
}