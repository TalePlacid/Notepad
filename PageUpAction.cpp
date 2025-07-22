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
	//1. ���� ��ũ�ѹٰ� ������,
	ScrollBarController* scrollBarController = ((NotepadForm*)(this->parent))->scrollBarController;
	if (scrollBarController->HasVSCrollBar())
	{
		//1.1. ���� ��ũ�ѹ� ������ �д´�.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

		//1.2. ���� ��ġ�� ��ũ�ѹ��� �ּҺ��� ũ��,
		if (scrollInfo.nPos > scrollInfo.nMin)
		{
			//1.2.1. ���� ���� �ʺ� ���Ѵ�.
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

			//1.2.2. �������� �ټ��� ���Ѵ�.
			Long pageRowCount = scrollInfo.nPage / sizeCalculator->GetRowHeight();

			//1.2.3. ����¡ ���ۿ��� �̵��Ѵ�.
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			Position current = pagingBuffer->GetCurrent();
			pagingBuffer->MoveRow(current.GetRow() - pageRowCount);

			//1.2.4. ������ �������� ����ٸ�, �������Ѵ�.
			if (!pagingBuffer->IsBelowTopLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
			}

			//1.2.5. �ٿ��� �ʺ�� ���� ����� ĭ���� �̵��Ѵ�.
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

			//1.2.6. ��ũ�ѹ� ��ġ�� �����Ѵ�.
			((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

			//1.2.7. Ŭ���̾�Ʈ ������ �����Ѵ�.
			note->Select(FALSE);
			pagingBuffer->UnmarkSelectionBegin();
			this->parent->Invalidate();
		}

	}
}