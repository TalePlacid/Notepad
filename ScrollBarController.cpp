#include <afxwin.h>
#include "ScrollBarController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ScrollBarController::ScrollBarController(CWnd* parent) {
	this->parent = parent;
	this->hasVScrollBar = FALSE;
	this->hasHScrollBar = FALSE;
}

ScrollBarController::~ScrollBarController() {

}

Long ScrollBarController::PageUp() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	BOOL hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	Long nPos = -1;
	if (hasScrollBar)
	{
		nPos = scrollInfo.nPos - scrollInfo.nPage;
		SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, nPos, TRUE);
	}

	return nPos;
}

Long ScrollBarController::PageDown() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	BOOL hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	Long nPos = -1;
	if (hasScrollBar)
	{
		nPos = scrollInfo.nPos + scrollInfo.nPage;
		SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, nPos, TRUE);
	}

	return nPos;
}

void ScrollBarController::Update(Subject* subject, string interest) {
	if (interest == "CreateScrollBars")
	{
		//1. 화면 크기를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		//2. 수직 스크롤바의 정보를 읽는다.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

		//3. 수직 스크롤바가 있으면,
		if (this->hasVScrollBar)
		{
			//3.1. 수직 스크롤바의 최대높이가 화면높이보다 작으면,
			if (scrollInfo.nMax <= clientAreaHeight)
			{
				//3.1.1. 수직 스크롤바의 지운다.
				this->parent->ModifyStyle(WS_VSCROLL, 0);
				this->hasVScrollBar = FALSE;

				//3.1.2. 수평 스크롤바가 있으면, 페이지크기를 갱신한다.
				GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
				if (this->hasHScrollBar)
				{
					scrollInfo.nPage += GetSystemMetrics(SM_CXVSCROLL);
					SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
				}
			}
		}
		else //4. 수직 스크롤바가 없으면,
		{
			//4.1. 메모 전체 높이를 구한다.
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			Long totalRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());
			Long totalHeight = totalRowCount * sizeCalculator->GetRowHeight();

			//4.2 메모 전체 높이가 화면높이보다 크면,
			if (totalHeight > clientAreaHeight)
			{
				//4.2.1. 수직 스크롤바를 만든다.
				this->parent->ModifyStyle(0, WS_VSCROLL);
				this->hasVScrollBar = TRUE;

				//4.2.2. 수직 스크롤바 정보를 갱신한다.
				scrollInfo.nMin = 0;
				scrollInfo.nMax = totalHeight;
				scrollInfo.nPage = clientAreaHeight;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

				//4.2.3. 수평 스크롤바가 있으면, 페이지크기를 갱신한다.
				GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
				if (this->hasHScrollBar)
				{
					scrollInfo.nPage -= GetSystemMetrics(SM_CXVSCROLL);
					SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
				}
			}

			//5. 수평 스크롤바 정보를 읽는다.
			GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

			//6. 수평 스크롤바가 없으면,
			if (!this->hasHScrollBar)
			{
				//6.1. 노트에서 첫 줄부터 끝 줄까지 반복한다.
				Long maxWidth = 0;
				Long width;
				Glyph* character;
				Glyph* row;
				Glyph* note = ((NotepadForm*)(this->parent))->note;
				Long i = 0;
				while (i < note->GetLength())
				{
					//6.1.1. 최대너비를 구한다.
					row = note->GetAt(i);
					width = 0;
					Long j = 0;
					while (j < row->GetLength())
					{
						character = row->GetAt(j);
						width += sizeCalculator->GetCharacterWidth((char*)(*character));
						j++;
					}

					if (width > maxWidth)
					{
						maxWidth = width;
					}
					i++;
				}

				//6.2. 최대너비가 화면너비보다 크면,
				if (maxWidth > clientAreaWidth)
				{
					//6.2.1. 수평 스크롤바를 만든다.
					this->parent->ModifyStyle(0, WS_HSCROLL);
					this->hasHScrollBar = TRUE;

					//6.2.2. 수평 스크롤바의 정보를 갱신한다.
					scrollInfo.nMin = 0;
					scrollInfo.nMax = maxWidth;
					scrollInfo.nPage = clientAreaWidth;
					scrollInfo.nPos = 0;
					SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

					//6.2.3. 수직 스크롤바가 있으면, 페이지크기를 갱신한다.
					GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
					if (this->hasVScrollBar)
					{
						scrollInfo.nPage -= GetSystemMetrics(SM_CYHSCROLL);
						SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
					}
				}
			}

		}
#if 0
		//1. 화면 너비를 구한다.
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		//2. 수직 스크롤바의 정보를 읽는다.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		BOOL hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

		//3. 페이징 버퍼에서 전체 줄 수를 읽는다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long totalRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());

		//4. 화면에 보이는 줄 수를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();

		//5. 수직 스크롤바가 있으면,
		Long scrollBarSize;
		if (hasScrollBar)
		{
			//5.1. 전체 줄 수가 화면에 보이는 줄 수보다 작으면, 수직 스크롤바를 지운다. 
			if (totalRowCount <= rowCount)
			{
				this->parent->ModifyStyle(WS_VSCROLL, 0);
			}
		}
		else //6. 수직 스크롤 바가 없으면,
		{
			//6.1. 메모의 전체 높이를 구한다.
			Long totalHeight = totalRowCount * sizeCalculator->GetRowHeight();

			//4.3. 전체높이가 화면높이보다 크면, 스크롤바를 만든다.
			if (totalHeight > clientAreaHeight)
			{
				this->parent->ModifyStyle(0, WS_VSCROLL);

				scrollInfo.nMin = 0;
				scrollInfo.nMax = totalHeight;
				scrollBarSize = GetSystemMetrics(SM_CYHSCROLL);
				scrollInfo.nPage = clientAreaHeight - scrollBarSize;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
			}
		}

		//5. 수평 스크롤바 정보를 읽는다.
		hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

		//6. 수평스크롤바가 없으면,
		if (!hasScrollBar)
		{

		}
#endif
#if 0
		Long maxWidth = 0;
		if (hasScrollBar)
		{
			maxWidth = scrollInfo.nMax;
		}

		//6. 노트에서 첫줄부터 끝줄까지 순회한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Glyph* row;
		Glyph* character;
		Long width;
		Long j;
		Long i = 0;
		while (i < note->GetLength())
		{
			//6.1. 제일 긴 줄의 너비를 구한다.
			row = note->GetAt(i);
			width = 0;
			j = 0;
			while (j < row->GetLength())
			{
				character = row->GetAt(j);
				width += sizeCalculator->GetCharacterWidth((char*)(*character));
				j++;
			}

			if (width > maxWidth)
			{
				maxWidth = width;
			}

			i++;
		}

		//7. 최대 너비가 화면 너비보다 작거나 같으면,
		if (maxWidth <= clientAreaWidth)
		{
			//7.1. 수평 스크롤바를 지운다.
			this->parent->ModifyStyle(WS_HSCROLL, 0);
		}
		else //8. 최대 너비가 화면 너비보다 크면,
		{
			//8.1. 수평 스크롤바가 없으면,
			if (!hasScrollBar)
			{
				//8.1.1. 수평 스크롤바를 만든다.
				this->parent->ModifyStyle(0, WS_HSCROLL);
				scrollInfo.nMin = 0;
				scrollBarSize = GetSystemMetrics(SM_CXVSCROLL);
				scrollInfo.nPage = clientAreaWidth - scrollBarSize;
				scrollInfo.nPos = 0;
			}

			//8.2. 수평 스크롤바 정보를 갱신한다.
			scrollInfo.nMax = maxWidth;
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
		}
#endif
		//9. 원도우를 갱신한다.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}