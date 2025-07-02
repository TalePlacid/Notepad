#include <afxwin.h>
#include "PageUpAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

PageUpAction::PageUpAction(CWnd* parent)
	:KeyAction(parent) {

}

PageUpAction::~PageUpAction() {

}

void PageUpAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	BOOL hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	if (hasScrollBar)
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowCount = scrollInfo.nPage / sizeCalculator->GetRowHeight();
		
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* currentRow = note->GetAt(rowIndex);
		Long columnIndex = currentRow->GetCurrent();

		Long upRowIndex = rowIndex - rowCount;
		upRowIndex = note->Move(upRowIndex);

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		pagingBuffer->MoveRow(upRowIndex);

		Glyph* row = note->GetAt(upRowIndex);
		Long rowWidth = 0;
		Glyph* character;
		Long i = 0;
		while (i < columnIndex)
		{
			character = currentRow->GetAt(i);
			rowWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		Long previousSum = 0;
		Long widthSum = 0;
		i = 0;
		while (i < row->GetLength() && widthSum <= rowWidth)
		{
			character = row->GetAt(i);
			previousSum = widthSum;
			widthSum += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		if (i < row->GetLength())
		{
			if (widthSum - rowWidth > rowWidth - previousSum)
			{
				i--;
			}

			columnIndex = row->Move(i);
			pagingBuffer->Move(i);
		}
		else
		{
			row->Last();
			pagingBuffer->Last();
		}
#if 0
		if (!pagingBuffer->IsOnPage())
		{
			pagingBuffer->Load();
		}
#endif
		note->Select(false);
		this->parent->Invalidate();
	}
}