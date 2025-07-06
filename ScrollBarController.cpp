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
		//1. ȭ�� ũ�⸦ ���Ѵ�.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		//2. ���� ��ũ�ѹ��� ������ �д´�.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

		//3. ���� ��ũ�ѹٰ� ������,
		if (this->hasVScrollBar)
		{
			//3.1. ���� ��ũ�ѹ��� �ִ���̰� ȭ����̺��� ������,
			if (scrollInfo.nMax <= clientAreaHeight)
			{
				//3.1.1. ���� ��ũ�ѹ��� �����.
				this->parent->ModifyStyle(WS_VSCROLL, 0);
				this->hasVScrollBar = FALSE;

				//3.1.2. ���� ��ũ�ѹٰ� ������, ������ũ�⸦ �����Ѵ�.
				GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
				if (this->hasHScrollBar)
				{
					scrollInfo.nPage += GetSystemMetrics(SM_CXVSCROLL);
					SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
				}
			}
		}
		else //4. ���� ��ũ�ѹٰ� ������,
		{
			//4.1. �޸� ��ü ���̸� ���Ѵ�.
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			Long totalRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());
			Long totalHeight = totalRowCount * sizeCalculator->GetRowHeight();

			//4.2 �޸� ��ü ���̰� ȭ����̺��� ũ��,
			if (totalHeight > clientAreaHeight)
			{
				//4.2.1. ���� ��ũ�ѹٸ� �����.
				this->parent->ModifyStyle(0, WS_VSCROLL);
				this->hasVScrollBar = TRUE;

				//4.2.2. ���� ��ũ�ѹ� ������ �����Ѵ�.
				scrollInfo.nMin = 0;
				scrollInfo.nMax = totalHeight;
				scrollInfo.nPage = clientAreaHeight;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

				//4.2.3. ���� ��ũ�ѹٰ� ������, ������ũ�⸦ �����Ѵ�.
				GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
				if (this->hasHScrollBar)
				{
					scrollInfo.nPage -= GetSystemMetrics(SM_CXVSCROLL);
					SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
				}
			}

			//5. ���� ��ũ�ѹ� ������ �д´�.
			GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

			//6. ���� ��ũ�ѹٰ� ������,
			if (!this->hasHScrollBar)
			{
				//6.1. ��Ʈ���� ù �ٺ��� �� �ٱ��� �ݺ��Ѵ�.
				Long maxWidth = 0;
				Long width;
				Glyph* character;
				Glyph* row;
				Glyph* note = ((NotepadForm*)(this->parent))->note;
				Long i = 0;
				while (i < note->GetLength())
				{
					//6.1.1. �ִ�ʺ� ���Ѵ�.
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

				//6.2. �ִ�ʺ� ȭ��ʺ񺸴� ũ��,
				if (maxWidth > clientAreaWidth)
				{
					//6.2.1. ���� ��ũ�ѹٸ� �����.
					this->parent->ModifyStyle(0, WS_HSCROLL);
					this->hasHScrollBar = TRUE;

					//6.2.2. ���� ��ũ�ѹ��� ������ �����Ѵ�.
					scrollInfo.nMin = 0;
					scrollInfo.nMax = maxWidth;
					scrollInfo.nPage = clientAreaWidth;
					scrollInfo.nPos = 0;
					SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

					//6.2.3. ���� ��ũ�ѹٰ� ������, ������ũ�⸦ �����Ѵ�.
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
		//1. ȭ�� �ʺ� ���Ѵ�.
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		//2. ���� ��ũ�ѹ��� ������ �д´�.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		BOOL hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

		//3. ����¡ ���ۿ��� ��ü �� ���� �д´�.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long totalRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());

		//4. ȭ�鿡 ���̴� �� ���� ���Ѵ�.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();

		//5. ���� ��ũ�ѹٰ� ������,
		Long scrollBarSize;
		if (hasScrollBar)
		{
			//5.1. ��ü �� ���� ȭ�鿡 ���̴� �� ������ ������, ���� ��ũ�ѹٸ� �����. 
			if (totalRowCount <= rowCount)
			{
				this->parent->ModifyStyle(WS_VSCROLL, 0);
			}
		}
		else //6. ���� ��ũ�� �ٰ� ������,
		{
			//6.1. �޸��� ��ü ���̸� ���Ѵ�.
			Long totalHeight = totalRowCount * sizeCalculator->GetRowHeight();

			//4.3. ��ü���̰� ȭ����̺��� ũ��, ��ũ�ѹٸ� �����.
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

		//5. ���� ��ũ�ѹ� ������ �д´�.
		hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

		//6. ����ũ�ѹٰ� ������,
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

		//6. ��Ʈ���� ù�ٺ��� ���ٱ��� ��ȸ�Ѵ�.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Glyph* row;
		Glyph* character;
		Long width;
		Long j;
		Long i = 0;
		while (i < note->GetLength())
		{
			//6.1. ���� �� ���� �ʺ� ���Ѵ�.
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

		//7. �ִ� �ʺ� ȭ�� �ʺ񺸴� �۰ų� ������,
		if (maxWidth <= clientAreaWidth)
		{
			//7.1. ���� ��ũ�ѹٸ� �����.
			this->parent->ModifyStyle(WS_HSCROLL, 0);
		}
		else //8. �ִ� �ʺ� ȭ�� �ʺ񺸴� ũ��,
		{
			//8.1. ���� ��ũ�ѹٰ� ������,
			if (!hasScrollBar)
			{
				//8.1.1. ���� ��ũ�ѹٸ� �����.
				this->parent->ModifyStyle(0, WS_HSCROLL);
				scrollInfo.nMin = 0;
				scrollBarSize = GetSystemMetrics(SM_CXVSCROLL);
				scrollInfo.nPage = clientAreaWidth - scrollBarSize;
				scrollInfo.nPos = 0;
			}

			//8.2. ���� ��ũ�ѹ� ������ �����Ѵ�.
			scrollInfo.nMax = maxWidth;
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
		}
#endif
		//9. �����츦 �����Ѵ�.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}