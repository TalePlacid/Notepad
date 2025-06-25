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

		//3. ���� ��ũ�ѹٰ� ������,
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (!hasScrollBar)
		{
			//3.1. ����¡���ۿ��� �� ���� �д´�.
			Long rowCount = pagingBuffer->CountRow();

			//3.2. �޸��� ��ü ���̸� ���Ѵ�.
			Long totalHeight = rowCount * sizeCalculator->GetRowHeight();

			//3.3. ��ü���̰� ȭ����̺��� ũ��, ��ũ�ѹٸ� �����.
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

		//4. ����¡ ���ۿ��� ���� �ٰ� �� ���� ��ġ�� �д´�.
		Long startRow = pagingBuffer->GetStart().GetRow();
		Long endRow = pagingBuffer->GetEnd().GetRow();

		//5. �� �ٱ����� ���̰� ȭ�� ���̺��� �۰ų� ������,
		Long endRowHeight = endRow * sizeCalculator->GetRowHeight();
		if (endRowHeight <= clientAreaHeight)
		{
			//5.1. ���� ��ũ�ѹٸ� �����.
			this->parent->ModifyStyle(WS_VSCROLL, 0);
		}
		else //6. �� �ٱ����� ���̰� ȭ�� ���̺��� ũ��,
		{
			//6.1. ���� ��ũ�ѹ� ������ �����Ѵ�.
			Long startRowHeight = startRow * sizeCalculator->GetRowHeight();
			scrollInfo.nPage = clientAreaHeight;
			scrollInfo.nPos = startRowHeight;
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
		}

		//7. ���� ��ũ�ѹ� ������ �д´�.
		hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

		Long maxWidth = 0;
		if (hasScrollBar)
		{
			maxWidth = scrollInfo.nMax;
		}

		//8. ��Ʈ���� ù�ٺ��� ���ٱ��� ��ȸ�Ѵ�.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Glyph* row;
		Glyph* character;
		Long width;
		Long j;
		Long i = 0;
		while (i < note->GetLength())
		{
			//8.1. ���� �� ���� �ʺ� ���Ѵ�.
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

		//9. �ִ� �ʺ� ȭ�� �ʺ񺸴� �۰ų� ������,
		if (maxWidth <= clientAreaWidth)
		{
			//9.1. ���� ��ũ�ѹٸ� �����.
			this->parent->ModifyStyle(WS_HSCROLL, 0);
		}
		else //10. �ִ� �ʺ� ȭ�� �ʺ񺸴� ũ��,
		{
			//10.1. ���� ��ũ�ѹٰ� ������,
			if (!hasScrollBar)
			{
				//10.1.1. ���� ��ũ�ѹٸ� �����.
				this->parent->ModifyStyle(0, WS_HSCROLL);
				scrollInfo.nMin = 0;
				scrollInfo.nPage = clientAreaWidth;
				scrollInfo.nPos = 0;
			}

			//10.2. ���� ��ũ�ѹ� ������ �����Ѵ�.
			scrollInfo.nMax = maxWidth;
			SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
		}

		//11. �����츦 �����Ѵ�.
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}