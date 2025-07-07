#include <afxwin.h>
#include "PageDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollBarController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

PageDownAction::PageDownAction(CWnd* parent)
	:KeyAction(parent) {

}

PageDownAction::~PageDownAction() {

}

void PageDownAction::Perform() {
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
	Long pageRange = scrollInfo.nPos + scrollInfo.nPage;

	ScrollBarController* scrollBarController = ((NotepadForm*)(this->parent))->scrollBarController;

	if (scrollBarController->HasVSCrollBar() && pageRange < scrollInfo.nMax)
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowCount = scrollInfo.nPage / sizeCalculator->GetRowHeight();
		
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* currentRow = note->GetAt(rowIndex);

		Glyph* character;
		Long rowWidth = 0;
		Long columnIndex = currentRow->GetCurrent();
		Long i = 0;
		while (i < columnIndex)
		{
			character = currentRow->GetAt(i);
			rowWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		Long downRowIndex = rowIndex + rowCount;
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Position current = pagingBuffer->MoveRow(downRowIndex);

		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
			note = ((NotepadForm*)(this->parent))->note;
			downRowIndex = pagingBuffer->GetCurrent().GetRow();
		}
		downRowIndex = note->Move(downRowIndex);

		Long previousWidth = 0;
		Long widthSum = 0;
		Glyph* row = note->GetAt(downRowIndex);
		i = 0;
		while (widthSum < rowWidth && i < row->GetLength())
		{
			character = row->GetAt(i);
			previousWidth = widthSum;
			widthSum += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}
		
		if (widthSum - rowWidth > rowWidth - previousWidth)
		{
			i--;
		}

		pagingBuffer->Move(i);
		row->Move(i);
		((NotepadForm*)(this->parent))->scrollBarController->PageDown();

		((NotepadForm*)(this->parent))->note->Select(false);
		this->parent->Invalidate();
	}
}