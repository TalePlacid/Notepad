#include <afxwin.h>
#include "PageDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollBarController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

PageDownAction::PageDownAction(CWnd* parent)
	:KeyAction(parent) {

}

PageDownAction::~PageDownAction() {

}

void PageDownAction::Perform() {
	//1. 수직 스크롤바가 존재하면,
	ScrollBarController* scrollBarController = ((NotepadForm*)(this->parent))->scrollBarController;
	if (scrollBarController->HasVSCrollBar())
	{
		//1.1. 수직 스크롤바 정보를 읽는다.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
		Long thumbBoxEnd = scrollInfo.nPos + scrollInfo.nPage;

		//1.2. 수직스크롤바의 썸박스의 끝이 스크롤바의 최대값보다 작으면,
		if (thumbBoxEnd < scrollInfo.nMax)
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

			//1.2.3. 페이징 버퍼에서 페이지당 줄수만큼 이동한다.
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			Position current = pagingBuffer->GetCurrent();
			pagingBuffer->MoveRow(current.GetRow() + pageRowCount);

			//1.2.4. 페이징 범위를 넘어섰으면, 로드한다.
			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
			}

			//1.2.5. 노트에서 이동한다.
			rowIndex = pagingBuffer->GetCurrent().GetRow();
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);

			//1.2.6. 가장 너비가 가까운 칸으로 이동한다.
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

			//1.2.7. 스크롤바의 위치를 조정한다.
			((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

			//1.2.8. 클라이언트 영역을 갱신한다.
			this->parent->Invalidate();
		}
	}
}