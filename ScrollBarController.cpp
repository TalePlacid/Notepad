#include <afxwin.h>
#include "ScrollBarController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

ScrollBarController::ScrollBarController(CWnd* parent) {
	this->parent = parent;
}

ScrollBarController::~ScrollBarController() {

}

void ScrollBarController::Update(Subject* subject, string interest) {
	if (interest == "CreateScrollBars")
	{
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		Long maxWidth = 0;
		Long width;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long j;
		Glyph* row;
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long i = 0;
		while (i < note->GetLength())
		{
			row = note->GetAt(i);
			width = 0;
			j = 0;
			while (j < row->GetLength())
			{
				TCHAR(*character) = (char*)(*(row->GetAt(j)));
				width += sizeCalculator->GetCharacterWidth(const_cast<char*>(character));
				j++;
			}

			if (width > maxWidth)
			{
				maxWidth = width;
			}

			i++;
		}

		Long totalHeight = note->GetLength() * sizeCalculator->GetRowHeight();

		Long originalVScrollBarPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
		Long originalHScrollBarPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);

		if (maxWidth > clientAreaWidth)
		{
			this->parent->ModifyStyle(0, WS_HSCROLL);
			SCROLLINFO horizontalScrollInfo = { sizeof(SCROLLINFO), SIF_ALL, originalHScrollBarPos,
				maxWidth, clientAreaWidth, 0, 0 };
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &horizontalScrollInfo, TRUE);
		}
		else
		{
			this->parent->ModifyStyle(WS_HSCROLL, 0);
		}

		if (totalHeight > clientAreaHeight)
		{
			this->parent->ModifyStyle(0, WS_VSCROLL);
			SCROLLINFO verticalScrollInfo = { sizeof(SCROLLINFO), SIF_ALL, originalVScrollBarPos,
				totalHeight, clientAreaHeight, 0, 0};
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &verticalScrollInfo, TRUE);
		}
		else
		{
			this->parent->ModifyStyle(WS_VSCROLL, 0);
		}

		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}