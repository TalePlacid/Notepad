#include <afxwin.h>
#include "PageDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

PageDownAction::PageDownAction(CWnd* parent)
	:KeyAction(parent) {

}

PageDownAction::~PageDownAction() {

}

void PageDownAction::Perform() {
	//1. 수직 스크롤바가 존재하면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (scrollController->HasVScroll() && vScroll.GetPos() < posLimit)
	{
		//1. 내려갈 만큼의 줄 수를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long rowCount = vScroll.GetPage() / rowHeight;
		if (vScroll.GetPage() % rowHeight > 0)
		{
			rowCount++;
		}

		//2. 노트에서 현재 줄의 너비를 구한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Long originalRowWidth = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			originalRowWidth += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i)));
			i++;
		}
		
		//3. 노트에서 줄 수 만큼 내려간다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

		Long restedRowCount = 0;
		if (rowIndex + rowCount >= note->GetLength())
		{
			restedRowCount = rowIndex + rowCount - note->GetLength() + 1;
			rowCount -= restedRowCount;
		}
		rowIndex = note->Move(rowIndex + rowCount);
		row = note->GetAt(rowIndex);
		row->First();
		pagingBuffer->NextRow(rowCount);

		//4. 줄 수가 충분치 않다면,
		if (restedRowCount > 0)
		{
			//4.1. 적재한다.
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);

			//4.2. 나머지 줄 수 만큼 내려간다.
			rowIndex = note->GetCurrent() + restedRowCount;
			if (rowIndex >= note->GetLength())
			{
				rowIndex = note->GetLength() - 1;
			}
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->NextRow(restedRowCount);
		}

		//5. 줄에서 너비와 가장 가까운 위치로 이동한다.
		Long previousWidth = 0;
		Long width = 0;
		i = 0;
		while (width < originalRowWidth && i < row->GetLength())
		{
			previousWidth = width;
			width += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i)));
			i++;
		}

		columnIndex = i;
		if (originalRowWidth - previousWidth < width - originalRowWidth)
		{
			columnIndex = i - 1;
		}

		columnIndex = row->Move(columnIndex);

		//6. 적재범위를 벗어났으면 재적재한다.
		if (note->IsBelowBottomLine(rowIndex))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}

		//7. 스크롤을 내린다.
		Long pos = vScroll.GetPos() + vScroll.GetPage();
		if (pos > posLimit)
		{
			pos = posLimit;
		}
		scrollController->MoveVScroll(pos);
	}
}