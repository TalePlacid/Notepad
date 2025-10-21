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
}

ScrollController::~ScrollController() {

}

void ScrollController::Update(Subject* subject, string interest) {
	if (interest == "UpdateScrollBars")
	{
		// 1. ��ũ�ѹ� �ʿ� ���θ� üũ�Ѵ�.
		ScrollBarNecessityChecker scrollBarNecessityChecker(this->parent);
		bool vScrollNeeded;
		bool hScrollNeeded;
		scrollBarNecessityChecker.Check(vScrollNeeded, hScrollNeeded);

		RECT clientArea;
		GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
		Long clientAreaHeight = clientArea.bottom - clientArea.top;
		Long clientAreaWidth = clientArea.right - clientArea.left;

		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long currentRow = note->GetCurrent();
		Glyph* row = note->GetAt(currentRow);
		Long currentColumn = row->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

		SCROLLINFO scrollInfo = { 0, };
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;

		// 2. ���� ��ũ�ѹٰ� �ʿ��ϸ�,
		if (vScrollNeeded)
		{
			if (!this->hasVScroll)
			{
				PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
				Long rowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());

				this->vScroll.ResizeRange(rowCount * sizeCalculator->GetRowHeight());
				this->vScroll.ResizePage(clientAreaHeight);
				this->vScroll.Move(currentRow * sizeCalculator->GetRowHeight());

				scrollInfo.nMin = this->vScroll.GetMin();
				scrollInfo.nMax = this->vScroll.GetMax();
				scrollInfo.nPage = this->vScroll.GetPage();
				scrollInfo.nPos = this->vScroll.GetPos();

				SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
				this->parent->ModifyStyle(0, WS_VSCROLL);
				this->hasVScroll = true;
			}
		}
		else // 3. ���� ��ũ�ѹٰ� �ʿ������,
		{
			// 3.1. ���� ��ũ�ѹ� ��Ÿ���� ���ش�.
			this->parent->ModifyStyle(WS_VSCROLL, 0);
			this->hasVScroll = false;
		}

		// 4. ���� ��ũ�ѹٰ� �ʿ��ϸ�,
		if (hScrollNeeded)
		{
			// 4.1. ���� ���� ��ũ�ѹٰ� ������,
			if (!this->hasHScroll)
			{
				// 4.1.1. ���� ��ũ�ѹ� ��Ÿ���� �����Ѵ�.
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
				this->hScroll.Move(width);

				scrollInfo.nMin = this->hScroll.GetMin();
				scrollInfo.nMax = this->hScroll.GetMax();
				scrollInfo.nPage = this->hScroll.GetPage();
				scrollInfo.nPos = this->hScroll.GetPos();

				SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);
				this->parent->ModifyStyle(0, WS_HSCROLL);
				this->hasHScroll = true;
			}
		}
		else // 5. ���� ��ũ�ѹٰ� �ʿ������,
		{
			// 5.1. ���� ��ũ�ѹ� ��Ÿ���� ���ش�.
			this->parent->ModifyStyle(WS_HSCROLL, 0);
			this->hasHScroll = false;
		}
		
		this->parent->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

void ScrollController::ResizeVRange(Long max, Long min) {
	this->vScroll.ResizeRange(max, min);
}

Long ScrollController::ResizeVPage(Long page) {
	return this->vScroll.ResizePage(page);
}

void ScrollController::ResizeHRange(Long max, Long min) {
	this->hScroll.ResizeRange(max, min);
}

Long ScrollController::ResizeHPage(Long page) {
	return this->hScroll.ResizePage(page);
}