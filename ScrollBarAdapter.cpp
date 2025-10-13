#include "ScrollBarAdapter.h"

#pragma warning(disable:4996)

ScrollBarAdapter::ScrollBarAdapter(CWnd* parent) {
	this->parent = parent;
	this->hasVScrollBar = FALSE;
	this->hasHScrollBar = FALSE;
}

ScrollBarAdapter::~ScrollBarAdapter() {

}

BOOL ScrollBarAdapter::Create(UINT scrollBar, Long nMin, Long nMax, Long nPage, Long nPos) {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin = nMin;
	scrollInfo.nMax = nMax;
	scrollInfo.nPage = nPage;
	scrollInfo.nPos = nPos;

	DWORD barType;
	if (scrollBar == SB_VERT)
	{
		barType = WS_VSCROLL;
	}
	else
	{
		barType = WS_HSCROLL;
	}

	BOOL modified = this->parent->ModifyStyle(0, barType);
	if (modified)
	{
		if (scrollBar == SB_VERT)
		{
			this->hasVScrollBar = TRUE;
		}
		else
		{
			this->hasHScrollBar = TRUE;
		}
	}
	SetScrollInfo(this->parent->GetSafeHwnd(), scrollBar, &scrollInfo, TRUE);

	this->parent->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	return modified;
}

BOOL ScrollBarAdapter::Destroy(UINT scrollBar) {
	DWORD barType;
	if (scrollBar == SB_VERT)
	{
		barType = WS_VSCROLL;
	}
	else
	{
		barType = WS_HSCROLL;
	}

	BOOL modified = this->parent->ModifyStyle(barType, 0);
	if (scrollBar == SB_VERT)
	{
		this->hasVScrollBar = FALSE;
	}
	else
	{
		this->hasHScrollBar = FALSE;
	}

	this->parent->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	return modified;
}

Long ScrollBarAdapter::UpdateMax(UINT scrollBar, Long nMax) {
	SetScrollRange(this->parent->GetSafeHwnd(), scrollBar, 0, nMax, TRUE);
	
	int max;
	int min;
	BOOL succeed = GetScrollRange(this->parent->GetSafeHwnd(), scrollBar, &min, &max);
	if (!succeed)
	{
		max = 0;
	}

	return max;
}

Long ScrollBarAdapter::ResizePage(UINT scrollBar, Long nPage) {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_PAGE;
	scrollInfo.nPage = nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), scrollBar, &scrollInfo, TRUE);
	
	BOOL succeed = GetScrollInfo(this->parent->GetSafeHwnd(), scrollBar, &scrollInfo);
	Long page = scrollInfo.nPage;
	if (!succeed)
	{
		page = 0;
	}

	return page;
}

Long ScrollBarAdapter::Move(UINT scrollBar, Long nPos) {
	SetScrollPos(this->parent->GetSafeHwnd(), scrollBar, nPos, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), scrollBar);
}

Long ScrollBarAdapter::Up(Long distance) {
	Long current = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, current + distance, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollBarAdapter::Down(Long distance) {
	Long current = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, current - distance, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollBarAdapter::Left(Long distance) {
	Long current = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, current - distance, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
}

Long ScrollBarAdapter::Right(Long distance) {
	Long current = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, current + distance, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
}

Long ScrollBarAdapter::PageUp() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;

	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
	scrollInfo.nPos += scrollInfo.nPage;

	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollBarAdapter::PageDown() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;

	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
	scrollInfo.nPos -= scrollInfo.nPage;

	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	return GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
}

Long ScrollBarAdapter::GetMax(UINT scrollBar) const {
	int min = 0;
	int max = 0;

	if ((scrollBar == SB_VERT && this->hasVScrollBar) || (scrollBar == SB_HORZ && this->hasHScrollBar))
	{
		GetScrollRange(this->parent->GetSafeHwnd(), scrollBar, &min, &max);
	}	

	return max;
}

Long ScrollBarAdapter::GetPage(UINT scrollBar) const {
	Long page = 0;
	if ((scrollBar == SB_VERT && this->hasVScrollBar) || (scrollBar == SB_HORZ && this->hasHScrollBar))
	{
		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_PAGE;
		GetScrollInfo(this->parent->GetSafeHwnd(), scrollBar, &scrollInfo);

		page = scrollInfo.nPage;
	}

	return page;
}

Long ScrollBarAdapter::GetPos(UINT scrollBar) const {
	return GetScrollPos(this->parent->GetSafeHwnd(), scrollBar);
}