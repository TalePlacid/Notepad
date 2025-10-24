#include <afxwin.h>
#include "ScrollController.h"
#include "ScrollBarNecessityChecker.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "Glyph.h"

#pragma warning(disable:4996)

ScrollController::ScrollController(CWnd* parent) {
	this->parent = parent;
	this->hasVScroll = false;
	this->hasHScroll = false;

	RECT clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
	Long clientAreaWidth = clientArea.right - clientArea.left;
	Long clientAreaHeight = clientArea.bottom - clientArea.top;

	this->vScroll.ResizePage(clientAreaHeight);
	this->hScroll.ResizePage(clientAreaWidth);
}

ScrollController::~ScrollController() {

}

void ScrollController::Update(Subject* subject, string interest) {
	if (interest == "UpdateScrollBars")
	{
		// 1. 스크롤바 필요 여부를 체크한다.
		ScrollBarNecessityChecker scrollBarNecessityChecker(this->parent);
		bool vScrollNeeded;
		bool hScrollNeeded;
		scrollBarNecessityChecker.Check(vScrollNeeded, hScrollNeeded);

		RECT clientArea;
		GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
		Long clientAreaHeight = clientArea.bottom - clientArea.top;
		Long clientAreaWidth = clientArea.right - clientArea.left;
		Long scrollBarThickness = GetSystemMetrics(SM_CXVSCROLL);

		if (vScrollNeeded)
		{
			clientAreaWidth -= scrollBarThickness;
		}

		if (hScrollNeeded)
		{
			clientAreaHeight -= scrollBarThickness;
		}

		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long currentRow = note->GetCurrent();
		Glyph* row = note->GetAt(currentRow);
		Long currentColumn = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;

		// 2. 수직 스크롤바가 필요하면,
		if (vScrollNeeded)
		{
			if (!this->hasVScroll)
			{
				PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
				Long rowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());

				this->vScroll.ResizeRange(rowCount * sizeCalculator->GetRowHeight());
				this->vScroll.ResizePage(clientAreaHeight);

				scrollInfo.nMin = this->vScroll.GetMin();
				scrollInfo.nMax = this->vScroll.GetMax();
				scrollInfo.nPage = this->vScroll.GetPage();
				scrollInfo.nPos = this->vScroll.GetPos();

				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
				this->parent->ModifyStyle(0, WS_VSCROLL);
				this->hasVScroll = true;
			}
		}
		else // 3. 수직 스크롤바가 필요없으면,
		{
			// 3.1. 수직 스크롤바 스타일을 없앤다.
			this->parent->ModifyStyle(WS_VSCROLL, 0);
			this->hasVScroll = false;
		}

		// 4. 수평 스크롤바가 필요하면,
		if (hScrollNeeded)
		{
			// 4.1. 현재 수평 스크롤바가 없으면,
			if (!this->hasHScroll)
			{
				// 4.1.1. 수평 스크롤바 스타일을 설정한다.
				Long maxWidth = clientAreaWidth;
				Glyph* line;
				Long width;
				Long i = 0;
				while (i < note->GetLength())
				{
					line = note->GetAt(i);
					width = sizeCalculator->GetRowWidth(line->MakeString().c_str());
					if (width > maxWidth)
					{
						maxWidth = width;
					}
					i++;
				}

				width = 0;
				i = 0;
				while (i < currentColumn)
				{
					width += sizeCalculator->GetCharacterWidth((char*)(*(row->GetAt(i))));
					i++;
				}

				this->hScroll.ResizeRange(maxWidth);
				this->hScroll.ResizePage(clientAreaWidth);

				scrollInfo.nMin = this->hScroll.GetMin();
				scrollInfo.nMax = this->hScroll.GetMax();
				scrollInfo.nPage = this->hScroll.GetPage();
				scrollInfo.nPos = this->hScroll.GetPos();

				SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
				this->parent->ModifyStyle(0, WS_HSCROLL);
				this->hasHScroll = true;
			}
		}
		else // 5. 수평 스크롤바가 필요없으면,
		{
			// 5.1. 수평 스크롤바 스타일을 없앤다.
			this->parent->ModifyStyle(WS_HSCROLL, 0);
			this->hasHScroll = false;
		}
		
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

void ScrollController::ResizeVRange(Long max, Long min) {
	this->vScroll.ResizeRange(max, min);
}

Long ScrollController::ResizeVPage(Long page) {
	return this->vScroll.ResizePage(page);
}

void ScrollController::ResizeHRange(Long max, Long min) {
	this->hScroll.ResizeRange(max, min);
}

Long ScrollController::ResizeHPage(Long page) {
	return this->hScroll.ResizePage(page);
}

Long ScrollController::Down() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long pos = this->vScroll.LineDown(sizeCalculator->GetRowHeight());

	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);

	return pos;
}