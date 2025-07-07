#include <afxwin.h>
#include "PageUpAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "ScrollBarController.h"

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
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	ScrollBarController* scrollBarController = ((NotepadForm*)(this->parent))->scrollBarController;

	if (scrollBarController->HasVSCrollBar() && scrollInfo.nPos > scrollInfo.nMin)
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowCount = scrollInfo.nPage / sizeCalculator->GetRowHeight();
		
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* originalRow = note->GetAt(rowIndex);
		Long columnIndex = originalRow->GetCurrent();

		Long originalRowWidth = 0;
		Glyph* character;
		Long i = 0;
		while (i < columnIndex)
		{
			character = originalRow->GetAt(i);
			originalRowWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		Long upRowIndex = rowIndex - rowCount;
		
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Position current = pagingBuffer->MoveRow(upRowIndex);

		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
			note = ((NotepadForm*)(this->parent))->note;
			upRowIndex = pagingBuffer->GetCurrent().GetRow();
		}

		upRowIndex = note->Move(upRowIndex);
		Glyph* row = note->GetAt(upRowIndex);

		Long previousWidth = 0;
		Long widthSum = 0;
		i = 0;
		while (i < row->GetLength() && widthSum < originalRowWidth)
		{
			character = row->GetAt(i);
			previousWidth = widthSum;
			widthSum += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		if (widthSum - originalRowWidth > originalRowWidth - previousWidth)
		{
			i--;
		}

		pagingBuffer->Move(i);
		row->Move(i);
		((NotepadForm*)(this->parent))->scrollBarController->PageUp();

		note->Select(false);
		this->parent->Invalidate();
	}
}