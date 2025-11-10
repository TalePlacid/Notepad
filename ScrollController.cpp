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

bool ScrollController::IsOnVScrollRange() {
	bool ret = false;

	if (this->hasVScroll)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long rowStartIndex = pagingBuffer->GetRowStartIndex();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long pos = (rowStartIndex + rowIndex) * rowHeight;

		Long rangeStart = this->vScroll.GetPos();
		Long rangeEnd = rangeStart + this->vScroll.GetPage();
		if (pos >= rangeStart && pos + rowHeight <= rangeEnd)
		{
			ret = true;
		}
	}

	return ret;
}

bool ScrollController::IsOnHScrollRange() {
	bool ret = false;

	if (this->hasHScroll)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long width = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			width += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i)));
			i++;
		}

		Long characterWidth = 0;
		if (columnIndex < row->GetLength() - 1)
		{
			characterWidth = sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(columnIndex)));
		}

		Long rangeStart = this->hScroll.GetPos();
		Long rangeEnd = rangeStart + this->hScroll.GetPage();
		if (width >= rangeStart && width + characterWidth <= rangeEnd)
		{
			ret = true;
		}
	}

	return ret;
}

void ScrollController::ResizeVRange(Long max, Long min) {
	this->vScroll.ResizeRange(max, min);

	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_RANGE;
	scrollInfo.nMin = min;
	scrollInfo.nMax = max;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
}

Long ScrollController::ResizeVPage(Long page) {
	this->vScroll.ResizePage(page);

	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_PAGE;
	scrollInfo.nPage = page;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	return scrollInfo.nPage;
}

void ScrollController::ResizeHRange(Long max, Long min) {
	this->hScroll.ResizeRange(max, min);

	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_RANGE;
	scrollInfo.nMin = min;
	scrollInfo.nMax = max;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
}

Long ScrollController::ResizeHPage(Long page) {
	this->hScroll.ResizePage(page);

	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_PAGE;
	scrollInfo.nPage = page;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	return scrollInfo.nPage;
}

Long ScrollController::Up() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long pos = this->vScroll.LineUp(sizeCalculator->GetRowHeight());

	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollController::Down() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long pos = this->vScroll.LineDown(sizeCalculator->GetRowHeight());

	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollController::Left(Long distance) {
	Long pos = this->hScroll.LineUp(distance);

	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, pos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
}

Long ScrollController::Right(Long distance) {
	Long pos = this->hScroll.LineDown(distance);

	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, pos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
}

Long ScrollController::MoveVScroll(Long pos) {
	pos = this->vScroll.Move(pos);

	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollController::MoveHScroll(Long pos) {
	pos = this->hScroll.Move(pos);

	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, pos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
}