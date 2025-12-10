#include <afxwin.h>
#include "ShiftPageDownAction.h"
#include "PageDownAction.h"
#include "ScrollController.h"
#include "NotepadForm.h"
#include "MarkingHelper.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftPageDownAction::ShiftPageDownAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftPageDownAction::~ShiftPageDownAction() {

}

void ShiftPageDownAction::Perform() {
	//1. 수직 스크롤바가 존재하고, 마지막 위치가 아니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (scrollController->HasVScroll() && vScroll.GetPos() < posLimit)
	{
		//1.1 내려갈 만큼의 줄 수를 구한다.
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

		//1.3. 노트에서 뒷부분을 선택한다.
		MarkingHelper markingHelper(this->parent);
		markingHelper.Mark();
		row->SelectRange(columnIndex, row->GetLength());
	
		//1.4. 노트와 페이징버퍼에서 줄 수 만큼 내려간다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

		Long restedRowCount = 0;
		if (rowIndex + rowCount >= note->GetLength())
		{
			restedRowCount = rowIndex + rowCount - note->GetLength() + 1;
			rowCount -= restedRowCount;
		}
		Long rowIndexToMove = note->Move(rowIndex + rowCount);
		row = note->GetAt(rowIndexToMove);
		row->First();
		pagingBuffer->NextRow(rowCount);
		note->SelectRange(rowIndex, rowIndexToMove - 1);

		//1.5. 노트와 페이징버퍼에서 너비만큼 이동하며 선택한다.
		columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		columnIndex = row->Move(columnIndex);
		row->SelectRange(0, columnIndex);
		pagingBuffer->Next(columnIndex);

		//1.6. 줄수가 충분치 않다면,
		if (restedRowCount > 0)
		{
			//1.6.1. 적재한다.
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();

			//1.6.2. 나머지 줄 수 만큼 내려간다.
			rowIndexToMove = rowIndex + restedRowCount;
			if (rowIndexToMove >= note->GetLength())
			{
				rowIndexToMove = note->GetLength() - 1;
			}
			rowIndexToMove = note->Move(rowIndexToMove);
			row = note->GetAt(rowIndexToMove);
			row->First();
			pagingBuffer->NextRow(restedRowCount);

			//1.6.3. 줄들을 선택한다.
			note->SelectRange(rowIndex, rowIndexToMove - 1);

			//1.6.4. 노트와 페이징버퍼에서 너비만큼 이동하며 선택한다.
			columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
			columnIndex = row->Move(columnIndex);
			row->SelectRange(0, columnIndex);
			pagingBuffer->Next(columnIndex);
		}

		//1.6. 적재범위를 벗어났으면 재적재한다.
		if (note->IsBelowBottomLine(rowIndex))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}

		//1.7. 스크롤을 내린다.
		Long pos = vScroll.GetPos() + vScroll.GetPage();
		if (pos > posLimit)
		{
			pos = posLimit;
		}
		scrollController->MoveVScroll(pos);
	}
}