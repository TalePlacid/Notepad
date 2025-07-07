#include <afxwin.h>
#include "VScrollBarUpClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Caret.h"
#include "CaretController.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

VScrollBarUpClickAction::VScrollBarUpClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarUpClickAction::~VScrollBarUpClickAction() {

}

void VScrollBarUpClickAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
	
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	scrollInfo.nPos -= sizeCalculator->GetRowHeight();

	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long y = caret->GetY() + sizeCalculator->GetRowHeight();

	if (y > clientAreaHeight)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* originalRow = note->GetAt(rowIndex);
		Long columnIndex = originalRow->GetCurrent();

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

		for (i = 1; i <= 2; i++)
		{
			rowIndex = note->Previous();
			pagingBuffer->PreviousRow();
		}

		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
			note = ((NotepadForm*)(this->parent))->note;
			rowIndex = note->GetCurrent();
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