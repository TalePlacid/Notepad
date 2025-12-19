#include <afxwin.h>
#include "ScrollController.h"
#include "ScrollBarAnalyzer.h"
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
	this->vScroll.ResizeRange(clientAreaHeight);
	this->hScroll.ResizePage(clientAreaWidth);
	this->hScroll.ResizeRange(clientAreaWidth);
}

ScrollController::~ScrollController() {

}

void ScrollController::Update(Subject* subject, string interest) {
	if (interest == "UpdateScrollBars")
	{
		// 1. 스크롤바 필요 여부를 체크한다.
		ScrollBarAnalyzer scrollBarAnalyzer(this->parent);
		if (!((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			scrollBarAnalyzer.AnalyzeWithoutWrap();
		}
		else
		{
			scrollBarAnalyzer.AnalyzeWithWrap();
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
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (scrollBarAnalyzer.GetVScrollNeeded())
		{
			//2.1. 현재 수직 스크롤바가 없으면,
			if (!this->hasVScroll)
			{
				//2.1.1. 수직 스크롤바를 만든다.
				this->vScroll.ResizeRange(scrollBarAnalyzer.GetContentsHeight());
				this->vScroll.ResizePage(scrollBarAnalyzer.GetClientAreaHeight());

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
			this->vScroll.ResizePage(scrollBarAnalyzer.GetClientAreaHeight());
			this->vScroll.ResizeRange(scrollBarAnalyzer.GetClientAreaHeight());
			this->vScroll.Move(0);
			this->parent->ModifyStyle(WS_VSCROLL, 0);
			this->hasVScroll = false;
		}

		// 4. 수평 스크롤바가 필요하면,
		if (scrollBarAnalyzer.GetHScrollNeeded())
		{
			// 4.1. 현재 수평 스크롤바가 없으면,
			if (!this->hasHScroll)
			{
				// 4.1.1. 수평 스크롤바 스타일을 설정한다.
				this->hScroll.ResizeRange(scrollBarAnalyzer.GetContentsWidth());
				this->hScroll.ResizePage(scrollBarAnalyzer.GetClientAreaWidth());

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
			this->hScroll.ResizePage(scrollBarAnalyzer.GetClientAreaWidth());
			this->hScroll.ResizeRange(scrollBarAnalyzer.GetClientAreaWidth());
			this->hScroll.Move(0);
			this->parent->ModifyStyle(WS_HSCROLL, 0);
			this->hasHScroll = false;
		}

		//6. 수직 스크롤바가 있으면,
		if (this->hasVScroll)
		{
			//6.1. 수직 스크롤 페이지를 갱신한다.
			this->vScroll.ResizePage(scrollBarAnalyzer.GetClientAreaHeight());
			scrollInfo.fMask = SIF_PAGE;
			scrollInfo.nPage = scrollBarAnalyzer.GetClientAreaHeight();
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

			//6.2. 수직 스크롤 범위를 벗어났으면, 수직 스크롤 위치를 조정한다.
			Long rowStartIndex = pagingBuffer->GetRowStartIndex();
			Long rowHeight = sizeCalculator->GetRowHeight();
			Long pos = (rowStartIndex + currentRow) * rowHeight;
			Long rangeStart = this->vScroll.GetPos();
			Long rangeEnd = rangeStart + this->vScroll.GetPage();
			if (pos < rangeStart)
			{
				this->vScroll.Move(pos);
				SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);
			}

			if (pos + rowHeight > rangeEnd)
			{
				this->vScroll.Move(pos + rowHeight - vScroll.GetPage());
				SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos + rowHeight - vScroll.GetPage(), TRUE);
			}
		}

		//7. 수평 스크롤이 있으면,
		if (this->hasHScroll)
		{
			//7.1. 수평 스크롤바 페이지 크기와 최댓값을 갱신한다.
			this->hScroll.ResizePage(scrollBarAnalyzer.GetClientAreaWidth());
			this->hScroll.ResizeRange(scrollBarAnalyzer.GetContentsWidth());
			scrollInfo.fMask = SIF_ALL;
			scrollInfo.nMin = this->hScroll.GetMin();
			scrollInfo.nMax = this->hScroll.GetMax();
			scrollInfo.nPage = this->hScroll.GetPage();
			scrollInfo.nPos = this->hScroll.GetPos();
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

			//7.2. 수평 스크롤 범위를 벗어났으면, 수평 스크롤 위치를 조정한다.
			Long width = 0;
			Long i = 0;
			while (i < currentColumn && i < row->GetLength())
			{
				width += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i)));
				i++;
			}

			Long characterWidth = 0;
			if (currentColumn < row->GetLength())
			{
				characterWidth = sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(currentColumn)));
			}

			Long rangeStart = this->hScroll.GetPos();
			Long rangeEnd = rangeStart + this->hScroll.GetPage();
			if (width < rangeStart)
			{
				this->hScroll.Move(width);
				SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, width, TRUE);
			}

			if (width + characterWidth > rangeEnd)
			{
				this->hScroll.Move(width + characterWidth - hScroll.GetPage());
				SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, width + characterWidth - hScroll.GetPage(), TRUE);
			}
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

bool ScrollController::ShowVScroll(bool visible) {
	bool ret;
	if (visible)
	{
		ret = this->parent->ModifyStyle(0, WS_VSCROLL);
		this->hasVScroll = true;
	}
	else
	{
		ret = this->parent->ModifyStyle(WS_VSCROLL, 0);
		this->hasVScroll = false;
	}

	return ret;
}

bool ScrollController::ShowHScroll(bool visible) {
	bool ret;
	if (visible)
	{
		ret = this->parent->ModifyStyle(0, WS_HSCROLL);
		this->hasVScroll = true;
	}
	else
	{
		ret = this->parent->ModifyStyle(WS_HSCROLL, 0);
		this->hasVScroll = false;
	}

	return ret;
}

void ScrollController::ResizeVRange(Long max, Long min) {
	this->vScroll.ResizeRange(max, min);

	if (this->hasVScroll)
	{
		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_RANGE;
		scrollInfo.nMin = min;
		scrollInfo.nMax = max;
		SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
	}
}

Long ScrollController::ResizeVPage(Long page) {
	Long resized = this->vScroll.ResizePage(page);

	if (this->hasVScroll)
	{
		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_PAGE;
		scrollInfo.nPage = page;
		SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
		resized = scrollInfo.nPage;
	}

	return resized;
}

void ScrollController::ResizeHRange(Long max, Long min) {
	this->hScroll.ResizeRange(max, min);

	if (this->hasHScroll)
	{
		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_RANGE;
		scrollInfo.nMin = min;
		scrollInfo.nMax = max;
		SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
	}
}

Long ScrollController::ResizeHPage(Long page) {
	Long resized = this->hScroll.ResizePage(page);

	if (this->hasHScroll)
	{
		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_PAGE;
		scrollInfo.nPage = page;
		SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

		GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
		resized = scrollInfo.nPage;
	}

	return resized;
}

Long ScrollController::Up() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long pos = this->vScroll.LineUp(sizeCalculator->GetRowHeight());

	if (this->hasVScroll)
	{
		SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);
		pos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	}

	return pos;
}

Long ScrollController::Down() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long pos = this->vScroll.LineDown(sizeCalculator->GetRowHeight());

	if (this->hasVScroll)
	{
		SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);
		pos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	}

	return pos;
}

Long ScrollController::Left(Long distance) {
	Long pos = this->hScroll.LineUp(distance);

	if (this->hasHScroll)
	{
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, pos, TRUE);
		pos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	}

	return pos;
}

Long ScrollController::Right(Long distance) {
	Long pos = this->hScroll.LineDown(distance);

	if (this->hasHScroll)
	{
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, pos, TRUE);
		pos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	}

	return pos;
}

Long ScrollController::MoveVScroll(Long pos) {
	pos = this->vScroll.Move(pos);

	if (this->hasVScroll)
	{
		SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, pos, TRUE);
		pos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	}

	return pos;
}

Long ScrollController::MoveHScroll(Long pos) {
	pos = this->hScroll.Move(pos);

	if (this->hasHScroll)
	{
		SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, pos, TRUE);
		pos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	}

	return pos;
}