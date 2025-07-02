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
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		nPos = (scrollInfo.nPos + scrollInfo.nPage) / rowHeight * rowHeight;
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
		Long scrollBarSize;
		if (!hasScrollBar)
		{
			//3.1. 페이징버퍼에서 줄 수를 읽는다.
			Long rowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());

			//3.2. 메모의 전체 높이를 구한다.
			Long totalHeight = rowCount * sizeCalculator->GetRowHeight();

			//3.3. 전체높이가 화면높이보다 크면, 스크롤바를 만든다.
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
		else //4. 수직 스크롤 바가 있으면,
		{
			//4.1. 페이지크기가 파일크기보다 크거나 같으면, 스크롤바를 없앤다.
			if (pagingBuffer->GetEndOffset() >= pagingBuffer->GetFileEnd())
			{
				this->parent->ModifyStyle(WS_VSCROLL, 0);
			}
		}

		//5. 수평 스크롤바 정보를 읽는다.
		hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

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

		//9. 원도우를 갱신한다.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}