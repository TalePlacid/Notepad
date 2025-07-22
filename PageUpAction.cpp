#include <afxwin.h>
#include "PageUpAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "ScrollBarController.h"

#pragma warning(disable:4996)

PageUpAction::PageUpAction(CWnd* parent)
	:KeyAction(parent) {

}

PageUpAction::~PageUpAction() {

}

void PageUpAction::Perform() {
	//1. 수직 스크롤바가 있으면,
	ScrollBarController* scrollBarController = ((NotepadForm*)(this->parent))->scrollBarController;
	if (scrollBarController->HasVSCrollBar())
	{
		//1.1. 수직 스크롤바 정보를 읽는다.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

		//1.2. 현재 위치가 스크롤바의 최소보다 크면,
		if (scrollInfo.nPos > scrollInfo.nMin)
		{
			//1.2.1. 현재 줄의 너비를 구한다.
			Glyph* note = ((NotepadForm*)(this->parent))->note;
			Long rowIndex = note->GetCurrent();
			Glyph* row = note->GetAt(rowIndex);
			Long columnIndex = row->GetCurrent();

			SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
			Glyph* character;
			Long originalWidth = 0;
			Long i = 0;
			while (i < columnIndex)
			{
				character = row->GetAt(i);
				originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
				i++;
			}

			//1.2.2. 페이지당 줄수를 구한다.
			Long pageRowCount = scrollInfo.nPage / sizeCalculator->GetRowHeight();

			//1.2.3. 페이징 버퍼에서 이동한다.
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			Position current = pagingBuffer->GetCurrent();
			pagingBuffer->MoveRow(current.GetRow() - pageRowCount);

			//1.2.4. 페이지 범위에서 벗어났다면, 재적재한다.
			if (!pagingBuffer->IsBelowTopLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
			}

			//1.2.5. 줄에서 너비와 가장 가까운 칸으로 이동한다.
			rowIndex = pagingBuffer->GetCurrent().GetRow();
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);

			Long previousWidth = 0;
			Long width = 0;
			i = 0;
			while (i < row->GetLength() && width < originalWidth)
			{
				character = row->GetAt(i);
				previousWidth = width;
				width += sizeCalculator->GetCharacterWidth((char*)(*character));
				i++;
			}

			if (width - originalWidth > originalWidth - previousWidth)
			{
				i--;
			}

			row->Move(i);
			pagingBuffer->Move(i);

			//1.2.6. 스크롤바 위치를 조정한다.
			((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

			//1.2.7. 클라이언트 영역을 갱신한다.
			note->Select(FALSE);
			pagingBuffer->UnmarkSelectionBegin();
			this->parent->Invalidate();
		}

	}
}