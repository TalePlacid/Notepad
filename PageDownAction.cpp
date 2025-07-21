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
	//1. ���� ��ũ�ѹٰ� �����ϸ�,
	ScrollBarController* scrollBarController = ((NotepadForm*)(this->parent))->scrollBarController;
	if (scrollBarController->HasVSCrollBar())
	{
		//1.1. ���� ��ũ�ѹ� ������ �д´�.
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
		Long thumbBoxEnd = scrollInfo.nPos + scrollInfo.nPage;

		//1.2. ������ũ�ѹ��� ��ڽ��� ���� ��ũ�ѹ��� �ִ밪���� ������,
		if (thumbBoxEnd < scrollInfo.nMax)
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

			//1.2.3. ����¡ ���ۿ��� �������� �ټ���ŭ �̵��Ѵ�.
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			Position current = pagingBuffer->GetCurrent();
			pagingBuffer->MoveRow(current.GetRow() + pageRowCount);

			//1.2.4. ����¡ ������ �Ѿ����, �ε��Ѵ�.
			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
			}

			//1.2.5. ��Ʈ���� �̵��Ѵ�.
			rowIndex = pagingBuffer->GetCurrent().GetRow();
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);

			//1.2.6. ���� �ʺ� ����� ĭ���� �̵��Ѵ�.
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

			//1.2.7. ��ũ�ѹ��� ��ġ�� �����Ѵ�.
			((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

			//1.2.8. Ŭ���̾�Ʈ ������ �����Ѵ�.
			this->parent->Invalidate();
		}
	}
}