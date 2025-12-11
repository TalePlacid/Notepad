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

		//1.3. 노트와 페이징버퍼에서 줄 수 만큼 올라간다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

		Long restedRowCount = 0;
		if (rowIndex - rowCount < 0)
		{
			restedRowCount = rowCount - rowIndex;
			rowCount = rowIndex;
		}
		rowIndex = note->Move(rowIndex - rowCount);
		row = note->GetAt(rowIndex);
		row->First();
		pagingBuffer->PreviousRow(rowCount);

		//1.4. 줄 수가 충분치 않다면,
		if (restedRowCount > 0)
		{
			//1.4.1. 적재한다.
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);

			//1.4.2. 나머지 줄 수 만큼 내려간다.
			rowIndex = note->GetCurrent() - restedRowCount;
			if (rowIndex < 0)
			{
				rowIndex = 0;
			}
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->PreviousRow(restedRowCount);
		}

		//1.5. 줄에서 너비와 가장 가까운 위치로 이동한다.
		columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		columnIndex = row->Move(columnIndex);
		pagingBuffer->Next(columnIndex);

		//1.6. 적재범위를 벗어났으면 재적재한다.
		if (note->IsAboveTopLine(rowIndex))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
		}
	}
}