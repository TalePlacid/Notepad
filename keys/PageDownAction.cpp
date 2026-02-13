#include <afxwin.h>
#include "PageDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

PageDownAction::PageDownAction(CWnd* parent)
	:KeyAction(parent) {

}

PageDownAction::~PageDownAction() {

}

void PageDownAction::Perform() {
	//1. 수직 스크롤바가 존재하고, 마지막 위치가 이니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (scrollController->HasVScroll() && vScroll.GetPos() < posLimit)
	{
		//1.1. 내려갈 줄 수를 구한다.
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

		//1.3. 처음위치로 이동한다.
		Long movedIndex = row->First();
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		pagingBuffer->Previous(columnIndex - movedIndex);

		//1.4. 줄수 만큼, 마지막 줄이 아니면 반복한다.
		Glyph* previousRow;
		Long i = 0;
		while (i < rowCount && rowIndex < note->GetLength() - 1)
		{
			//1.4.1. 적재범위에서 벗어났으면, 재적재한다.
			Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * rowHeight;
			if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < vScroll.GetMax())
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
				rowIndex = note->GetCurrent();
			}

			//1.4.2. 노트에서 이동한다.
			previousRow = note->GetAt(rowIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			//1.4.3. 줄이 가짜 줄이면,
			if (row->IsDummyRow())
			{
				pagingBuffer->Next(previousRow->GetLength());
			}
			else
			{
				pagingBuffer->NextRow();
			}

			i++;
		}

		//1.5. 줄 너비와 가장 가까운 위치로 이동한다.
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		movedIndex = row->Move(nearestIndex);
		pagingBuffer->Next(movedIndex);
	}
}