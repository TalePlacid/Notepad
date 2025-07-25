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

		BOOL isHScrollBarCreated = FALSE;
		BOOL isVScrollBarCreated = FALSE;

		//3. 수직 스크롤바가 있으면,
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (this->hasVScrollBar)
		{
			//3.1. 페이징버퍼에서 전체높이를 구한다.
			Long totalHeight = (pagingBuffer->CountRow(pagingBuffer->GetFileEnd()) + 1)
				* sizeCalculator->GetRowHeight();

			//3.1. 수직 스크롤바의 최대높이가 화면높이보다 작으면,
			if (totalHeight <= clientAreaHeight)
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
			else if (totalHeight > scrollInfo.nMax)
			{
				scrollInfo.nMax = totalHeight;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
			}
		}
		else //4. 수직 스크롤바가 없으면,
		{
			//4.1. 메모 전체 높이를 구한다.
			Long totalRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());
			Long totalHeight = (totalRowCount + 1) * sizeCalculator->GetRowHeight();

			//4.2 메모 전체 높이가 화면높이보다 크면,
			if (totalHeight > clientAreaHeight)
			{
				//4.2.1. 수직 스크롤바를 만든다.
				this->parent->ModifyStyle(0, WS_VSCROLL);
				this->hasVScrollBar = TRUE;
				isVScrollBarCreated = TRUE;

				//4.2.2. 수직 스크롤바 정보를 갱신한다.
				scrollInfo.nMin = 0;
				scrollInfo.nMax = totalHeight;
				scrollInfo.nPage = clientAreaHeight;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
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
				isHScrollBarCreated = TRUE;

				//6.2.2. 수평 스크롤바의 정보를 갱신한다.
				scrollInfo.nMin = 0;
				scrollInfo.nMax = maxWidth;
				scrollInfo.nPage = clientAreaWidth;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
			}
		}

		//9. 수직 스크롤바가 만들어졌고, 수평 스크롤바가 있으면, 페이지크기를 갱신한다.
		if (isVScrollBarCreated && this->hasHScrollBar)
		{
			GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
			scrollInfo.nPage -= GetSystemMetrics(SM_CXVSCROLL);
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
		}

		//10. 수평 스크롤바가 만들어졌고, 수직 스크롤바가 있으면, 페이지 크기를 갱신한다.
		if (isHScrollBarCreated && this->hasVScrollBar)
		{
			GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
			scrollInfo.nPage -= GetSystemMetrics(SM_CYHSCROLL);
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
		}

		//11. 원도우를 갱신한다.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else if (interest == "AdjustScrollBars")
	{
		//1. 화면 크기를 읽는다.
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		//2. 노트에서 현재 위치를 읽는다.
		Glyph *note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//3. 현재 위치의 x좌표를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Glyph* character;
		Long width = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			character = row->GetAt(i);
			width += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		Long hScrollPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
		Long x = width - hScrollPos;

		//4. 현재 위치의 y좌표를 구한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long rowCount = pagingBuffer->CountRow(pagingBuffer->GetStartOffset())
			+ pagingBuffer->GetCurrent().GetRow();
		Long vScrollPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
		Long y = rowCount * sizeCalculator->GetRowHeight() - vScrollPos;

		Long nPos;
		if (x < 0 || x > clientAreaWidth)
		{
			if (x < 0)
			{
				nPos = hScrollPos + x;
			}
			else if (x > clientAreaWidth)
			{
				nPos = hScrollPos + (x - clientAreaWidth);
			}
			SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, nPos, TRUE);
		}
		
		if (y < 0 || y + sizeCalculator->GetRowHeight() > clientAreaHeight)
		{
			if (y < 0)
			{
				nPos = vScrollPos + y;
			}
			else if (y + sizeCalculator->GetRowHeight() > clientAreaHeight)
			{
				nPos = vScrollPos + ((y + sizeCalculator->GetRowHeight()) - clientAreaHeight);
			}
			SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, nPos, TRUE);
		}
	}
}