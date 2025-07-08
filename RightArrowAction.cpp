#include <afxwin.h>
#include "RightArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

RightArrowAction::RightArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

RightArrowAction::~RightArrowAction() {

}

void RightArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (row->GetCurrent() < row->GetLength())
	{
		row->Next();
		pagingBuffer->Next();
	}
	else
	{
		if (note->GetCurrent() < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
				rowIndex = note->GetCurrent();
			}

			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();
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
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, 0, TRUE);
	}
	else if (width > right)
	{
		character = row->GetAt(row->GetCurrent() - 1);
		nPos += sizeCalculator->GetCharacterWidth((char*)(*character));
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, nPos, TRUE);
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}