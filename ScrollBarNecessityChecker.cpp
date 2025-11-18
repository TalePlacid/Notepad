#include <afxwin.h>
#include "ScrollBarNecessityChecker.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

ScrollBarNecessityChecker::ScrollBarNecessityChecker(CWnd *parent) {
	this->parent = parent;
}

ScrollBarNecessityChecker::~ScrollBarNecessityChecker() {

}

void ScrollBarNecessityChecker::Check(bool& vScrollNeeded, bool& hScrollNeeded) {
	RECT clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
	Long clientAreaWidth = clientArea.right - clientArea.left;
	Long clientAreaHeight = clientArea.bottom - clientArea.top;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;

	Long contentsHeight = note->GetLength() * sizeCalculator->GetRowHeight();
	Long contentsWidth = 0;
	Long width;
	Glyph* row;
	Long i;
	if (scrollController->HasHScroll())
	{
		Long rowIndex = note->GetCurrent();
		i = rowIndex - PAGE_ROWCOUNT;
		if (i < 0)
		{
			i = 0;
		}

		Long j = rowIndex + PAGE_ROWCOUNT;
		if (j > note->GetLength() - 1)
		{
			j = note->GetLength() - 1;
		}

		while (i <= j)
		{
			row = note->GetAt(i);
			width = sizeCalculator->GetRowWidth(row->MakeString().c_str());
			if (width > contentsWidth)
			{
				contentsWidth = width;
			}
			i++;
		}
	}
	else
	{
		i = 0;
		while (i < note->GetLength())
		{
			row = note->GetAt(i);
			width = sizeCalculator->GetRowWidth(row->MakeString().c_str());
			if (width > contentsWidth)
			{
				contentsWidth = width;
			}
			i++;
		}
	}


	vScrollNeeded = contentsHeight > clientAreaHeight;
	hScrollNeeded = contentsWidth > clientAreaWidth;

	Long scrollBarThickness = GetSystemMetrics(SM_CXVSCROLL);
	for (Long i = 1; i <= 2; i++)
	{
		if (vScrollNeeded)
		{
			Long updatedClientAreaWidth = clientAreaWidth - scrollBarThickness;
			hScrollNeeded = contentsWidth > updatedClientAreaWidth;
		}

		if (hScrollNeeded)
		{
			Long updatedClientAreaHeight = clientAreaHeight - scrollBarThickness;
			vScrollNeeded = contentsHeight > updatedClientAreaHeight;
		}
	}
}