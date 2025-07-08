#include <afxwin.h>
#include "LeftArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

LeftArrowAction::LeftArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

LeftArrowAction::~LeftArrowAction() {

}

void LeftArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	Long columnIndex = row->GetCurrent();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		row->Previous();
		pagingBuffer->Previous();
	}
	else
	{
		if (rowIndex > 0)
		{
			rowIndex = note->Previous();
			pagingBuffer->PreviousRow();
			if (!pagingBuffer->IsBelowTopLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
				rowIndex = note->GetCurrent();
			}
			row = note->GetAt(rowIndex);
			row->Last();
			pagingBuffer->Last();
		}
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Glyph* character;
	Long width = 0;
	Long i = 0;
	while (i < row->GetCurrent())
	{
		character = row->GetAt(i);
		width += sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaWidth = rect.right - rect.left;

	Long nPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	Long left = nPos;
	Long right = nPos + clientAreaWidth;

	if (width < left)
	{
		character = row->GetAt(row->GetCurrent());
		nPos -= sizeCalculator->GetCharacterWidth((char*)(*character));
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, nPos, TRUE);
	}
	else if (width > right)
	{
		Long previousRowEnd = width - clientAreaWidth;
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, previousRowEnd, TRUE);
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}