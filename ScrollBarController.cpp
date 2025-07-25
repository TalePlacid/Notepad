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

		BOOL isHScrollBarCreated = FALSE;
		BOOL isVScrollBarCreated = FALSE;

		//3. ���� ��ũ�ѹٰ� ������,
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (this->hasVScrollBar)
		{
			//3.1. ����¡���ۿ��� ��ü���̸� ���Ѵ�.
			Long totalHeight = (pagingBuffer->CountRow(pagingBuffer->GetFileEnd()) + 1)
				* sizeCalculator->GetRowHeight();

			//3.1. ���� ��ũ�ѹ��� �ִ���̰� ȭ����̺��� ������,
			if (totalHeight <= clientAreaHeight)
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
			else if (totalHeight > scrollInfo.nMax)
			{
				scrollInfo.nMax = totalHeight;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
			}
		}
		else //4. ���� ��ũ�ѹٰ� ������,
		{
			//4.1. �޸� ��ü ���̸� ���Ѵ�.
			Long totalRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEnd());
			Long totalHeight = (totalRowCount + 1) * sizeCalculator->GetRowHeight();

			//4.2 �޸� ��ü ���̰� ȭ����̺��� ũ��,
			if (totalHeight > clientAreaHeight)
			{
				//4.2.1. ���� ��ũ�ѹٸ� �����.
				this->parent->ModifyStyle(0, WS_VSCROLL);
				this->hasVScrollBar = TRUE;
				isVScrollBarCreated = TRUE;

				//4.2.2. ���� ��ũ�ѹ� ������ �����Ѵ�.
				scrollInfo.nMin = 0;
				scrollInfo.nMax = totalHeight;
				scrollInfo.nPage = clientAreaHeight;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
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
				isHScrollBarCreated = TRUE;

				//6.2.2. ���� ��ũ�ѹ��� ������ �����Ѵ�.
				scrollInfo.nMin = 0;
				scrollInfo.nMax = maxWidth;
				scrollInfo.nPage = clientAreaWidth;
				scrollInfo.nPos = 0;
				SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
			}
		}

		//9. ���� ��ũ�ѹٰ� ���������, ���� ��ũ�ѹٰ� ������, ������ũ�⸦ �����Ѵ�.
		if (isVScrollBarCreated && this->hasHScrollBar)
		{
			GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);
			scrollInfo.nPage -= GetSystemMetrics(SM_CXVSCROLL);
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
		}

		//10. ���� ��ũ�ѹٰ� ���������, ���� ��ũ�ѹٰ� ������, ������ ũ�⸦ �����Ѵ�.
		if (isHScrollBarCreated && this->hasVScrollBar)
		{
			GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
			scrollInfo.nPage -= GetSystemMetrics(SM_CYHSCROLL);
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
		}

		//11. �����츦 �����Ѵ�.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else if (interest == "AdjustScrollBars")
	{
		//1. ȭ�� ũ�⸦ �д´�.
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaWidth = rect.right - rect.left;
		Long clientAreaHeight = rect.bottom - rect.top;

		//2. ��Ʈ���� ���� ��ġ�� �д´�.
		Glyph *note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//3. ���� ��ġ�� x��ǥ�� ���Ѵ�.
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

		//4. ���� ��ġ�� y��ǥ�� ���Ѵ�.
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