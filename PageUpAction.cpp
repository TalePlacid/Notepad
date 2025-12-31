#include <afxwin.h>
#include "PageUpAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

PageUpAction::PageUpAction(CWnd* parent)
	:KeyAction(parent) {

}

PageUpAction::~PageUpAction() {

}

void PageUpAction::Perform() {
	//1. 수직스크롤바가 있고, 처음 위치가 아니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	if (scrollController->HasVScroll() && vScroll.GetPos() > 0)
	{
		//1.1. 올라갈 줄 수를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long rowCount = vScroll.GetPage() / rowHeight;
		if (vScroll.GetPage() % rowHeight > 0)
		{
			rowCount++;
		}

		//1.2. 노트에서 현재 줄의 너비를 구한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Long originalRowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//1.3. 줄의 처음으로 이동한다.
		Long movedIndex = row->First();
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		pagingBuffer->Previous(columnIndex - movedIndex);

		//1.3. 줄 수 만큼, 첫번째 줄이 아니면 반복한다.
		Glyph* previousRow;
		Long i = 0;
		while (i < rowCount && rowIndex > 0)
		{
			//1.3.1. 적재범위를 벗어나면, 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
				rowIndex = note->GetCurrent();
			}

			//1.3.2. 노트에서 이동한다.
			previousRow = note->GetAt(rowIndex);
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			//1.3.3. 이전 줄이 진짜 줄이면,
			if (!previousRow->IsDummyRow())
			{
				//1.3.3.1. 페이징 버퍼에서 이전 줄로 이동한다.
				pagingBuffer->PreviousRow();

				//1.3.3.2. 현재 줄이 더미 줄이면, 추가적으로 이동한다.
				if (row->IsDummyRow())
				{
					pagingBuffer->Last();
					pagingBuffer->Previous(row->GetLength());
				}
			}
			else //1.3.4. 이전 줄이 더미 줄이면,
			{
				//1.3.4.1. 페이징 버퍼에서 이전으로 이동한다.
				pagingBuffer->Previous(row->GetLength());
			}

			i++;
		}

		//1.4. 줄 너비와 가장 가까운 위치로 이동한다.
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		movedIndex = row->Move(nearestIndex);
		pagingBuffer->Next(movedIndex);
	}
}