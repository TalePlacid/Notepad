#include <afxwin.h>
#include "NotepadForm.h"
#include "ScrollBarAnalyzer.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

ScrollBarAnalyzer::ScrollBarAnalyzer(CWnd *parent) {
	this->parent = parent;
	this->vScrollNeeded = false;
	this->hScrollNeeded = false;
	this->clientAreaHeight = 0;
	this->clientAreaWidth = 0;
	this->contentsHeight = 0;
	this->contentsWidth = 0;
}

ScrollBarAnalyzer::~ScrollBarAnalyzer() {

}

void ScrollBarAnalyzer::Analyze() {
	RECT clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
	this->clientAreaWidth = clientArea.right - clientArea.left;
	this->clientAreaHeight = clientArea.bottom - clientArea.top;

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	this->scrollBarThickness = GetSystemMetrics(SM_CXVSCROLL);
	if (scrollController->HasVScroll())
	{
		this->clientAreaWidth += this->scrollBarThickness;
	}

	if (scrollController->HasHScroll())
	{
		this->clientAreaHeight += this->scrollBarThickness;
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	Long rowHeight = sizeCalculator->GetRowHeight();
	Long rowCount = this->clientAreaHeight / rowHeight;
	if (rowCount == 0)
	{
		rowCount = 1;
	}

	this->contentsHeight = note->GetLength() * rowHeight;
	this->contentsWidth = 0;
	Long width;
	Glyph* row;
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		width = sizeCalculator->GetRowWidth(row->MakeString().c_str());
		if (width > this->contentsWidth)
		{
			this->contentsWidth = width;
		}
		i++;
	}
#if 0
	if (scrollController->HasHScroll())
	{
		Long rowIndex = note->GetCurrent();
		i = rowIndex - rowCount;
		if (i < 0)
		{
			i = 0;
		}

		Long j = rowIndex + rowCount;
		if (j > note->GetLength() - 1)
		{
			j = note->GetLength() - 1;
		}

		while (i <= j)
		{
			row = note->GetAt(i);
			width = sizeCalculator->GetRowWidth(row->MakeString().c_str());
			if (width > this->contentsWidth)
			{
				this->contentsWidth = width;
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
			if (width > this->contentsWidth)
			{
				this->contentsWidth = width;
			}
			i++;
		}
	}
#endif
	this->vScrollNeeded = this->contentsHeight > this->clientAreaHeight;
	this->hScrollNeeded = this->contentsWidth > this->clientAreaWidth;

	for (Long i = 1; i <= 2; i++)
	{
		if (this->vScrollNeeded)
		{
			Long updatedClientAreaWidth = this->clientAreaWidth - this->scrollBarThickness;
			this->hScrollNeeded = this->contentsWidth > updatedClientAreaWidth;
		}

		if (this->hScrollNeeded)
		{
			Long updatedClientAreaHeight = this->clientAreaHeight - this->scrollBarThickness;
			this->vScrollNeeded = this->contentsHeight > updatedClientAreaHeight;
		}
	}

	if (this->vScrollNeeded)
	{
		this->clientAreaWidth -= this->scrollBarThickness;
	}

	if (this->hScrollNeeded)
	{
		this->clientAreaHeight -= this->scrollBarThickness;
	}
}