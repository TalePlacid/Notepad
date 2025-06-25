#include <afxwin.h>
#include "ScrollBarController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ScrollBarController::ScrollBarController(CWnd* parent) {
	this->parent = parent;
}

ScrollBarController::~ScrollBarController() {

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

		//3. 수직 스크롤바가 없으면,
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (!hasScrollBar)
		{
			//3.1. 페이징버퍼에서 줄 수를 읽는다.
			Long rowCount = pagingBuffer->CountRow();

			//3.2. 메모의 전체 높이를 구한다.
			Long totalHeight = rowCount * sizeCalculator->GetRowHeight();

			//3.3. 전체높이가 화면높이보다 크면, 스크롤바를 만든다.
			if (totalHeight > clientAreaHeight)
			{
				this->parent->ModifyStyle(0, WS_VSCROLL);

				scrollInfo.nMin = 0;
				scrollInfo.nMax = totalHeight;
				scrollInfo.nPage = clientAreaHeight;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
			}
		}

		//4. 페이징 버퍼에서 시작 줄과 끝 줄의 위치을 읽는다.
		Long startRow = pagingBuffer->GetStart().GetRow();
		Long endRow = pagingBuffer->GetEnd().GetRow();

		//5. 끝 줄까지의 높이가 화면 높이보다 작거나 같으면,
		Long endRowHeight = endRow * sizeCalculator->GetRowHeight();
		if (endRowHeight <= clientAreaHeight)
		{
			//5.1. 수직 스크롤바를 지운다.
			this->parent->ModifyStyle(WS_VSCROLL, 0);
		}
		else //6. 끝 줄까지의 높이가 화면 높이보다 크면,
		{
			//6.1. 수직 스크롤바 정보를 갱신한다.
			Long startRowHeight = startRow * sizeCalculator->GetRowHeight();
			scrollInfo.nPage = clientAreaHeight;
			scrollInfo.nPos = startRowHeight;
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
		}

		//7. 수평 스크롤바 정보를 읽는다.
		hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

		Long maxWidth = 0;
		if (hasScrollBar)
		{
			maxWidth = scrollInfo.nMax;
		}

		//8. 노트에서 첫줄부터 끝줄까지 순회한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Glyph* row;
		Glyph* character;
		Long width;
		Long j;
		Long i = 0;
		while (i < note->GetLength())
		{
			//8.1. 제일 긴 줄의 너비를 구한다.
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

		//9. 최대 너비가 화면 너비보다 작거나 같으면,
		if (maxWidth <= clientAreaWidth)
		{
			//9.1. 수평 스크롤바를 지운다.
			this->parent->ModifyStyle(WS_HSCROLL, 0);
		}
		else //10. 최대 너비가 화면 너비보다 크면,
		{
			//10.1. 수평 스크롤바가 없으면,
			if (!hasScrollBar)
			{
				//10.1.1. 수평 스크롤바를 만든다.
				this->parent->ModifyStyle(0, WS_HSCROLL);
				scrollInfo.nMin = 0;
				scrollInfo.nPage = clientAreaWidth;
				scrollInfo.nPos = 0;
			}

			//10.2. 수평 스크롤바 정보를 갱신한다.
			scrollInfo.nMax = maxWidth;
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
		}

		//11. 원도우를 갱신한다.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}