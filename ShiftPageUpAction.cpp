#include <afxwin.h>
#include "ShiftPageUpAction.h"
#include "ScrollController.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

ShiftPageUpAction::ShiftPageUpAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftPageUpAction::~ShiftPageUpAction() {

}

void ShiftPageUpAction::Perform() {
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

		//1.2. 현재 너비를 구한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();
		Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		//1.3. 줄수만큼, 첫번째 줄위치보다 크면 반복한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long selectionBeginOffset;
		Long previousOffset = -1;
		Long currentOffset = pagingBuffer->GetCurrentOffset();

		Long previousRowIndex = -1;
		Long i = 1;
		while (i <= rowCount && rowIndex != previousRowIndex)
		{
			//1.3.1. 현재 줄에서 선택한다.
			while (previousOffset != currentOffset)
			{
				//1.3.1.1. 페이징 버퍼에서 마킹한다.
				selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
				if (selectionBeginOffset < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}
				else if (currentOffset == selectionBeginOffset)
				{
					pagingBuffer->UnmarkSelectionBegin();
				}

				//1.3.1.2. 노트에서 선택반전한다.
				if (columnIndex > 0)
				{
					row->GetAt(columnIndex - 1)->ToggleSelection();
				}

				previousOffset = currentOffset;
				currentOffset = pagingBuffer->Previous();
				if (currentOffset == selectionBeginOffset)
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
				columnIndex = row->Previous();
			}

			//1.3.2. 이전 줄이 적재범위를 넘어서면 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
				rowIndex = note->GetCurrent();
			}

			//1.3.3. 이전 줄로 이동한다.
			currentOffset = pagingBuffer->PreviousRow();
			previousRowIndex = rowIndex;
			rowIndex = note->Previous();
			i++;

			if (rowIndex != previousRowIndex)
			{
				//1.3.4. 너비에 근접한 열부터 뒷부분을 선택한다.
				currentOffset = pagingBuffer->Last();
				row = note->GetAt(rowIndex);
				columnIndex = row->Last();

				Long nearestColumnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
				Long j = row->GetLength();
				while (previousOffset != currentOffset && j > nearestColumnIndex)
				{
					//1.3.4.1. 페이징 버퍼에서 마킹한다.
					selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
					if (selectionBeginOffset < 0)
					{
						pagingBuffer->MarkSelectionBegin();
					}
					else if (currentOffset == selectionBeginOffset)
					{
						pagingBuffer->UnmarkSelectionBegin();
					}

					//1.3.4.2. 노트에서 선택반전한다.
					if (columnIndex > 0)
					{
						row->GetAt(columnIndex - 1)->ToggleSelection();
					}

					previousOffset = currentOffset;
					currentOffset = pagingBuffer->Previous();
					if (currentOffset == selectionBeginOffset)
					{
						pagingBuffer->UnmarkSelectionBegin();
					}
					columnIndex = row->Previous();
					j--;
				}
			}
		}
	}
#if 0
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

		//1.3. 현재줄의 앞을 선택한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		MarkingHelper markingHelper(this->parent);
		markingHelper.Mark();
		bool isSelecting = true;
		if (pagingBuffer->GetCurrentOffset() > pagingBuffer->GetSelectionBeginOffset())
		{
			isSelecting = false;
		}
		row->SelectRange(0, columnIndex, isSelecting);

		//1.4. 노트와 페이징버퍼에서 줄 수 만큼 올라간다.
		Long restedRowCount = 0;
		if (rowIndex - rowCount < 0)
		{
			restedRowCount = rowCount - rowIndex;
			rowCount = rowIndex;
		}
		Long rowIndexToMove = note->Move(rowIndex - rowCount);
		row = note->GetAt(rowIndexToMove);
		row->First();
		pagingBuffer->PreviousRow(rowCount);
		note->SelectRange(rowIndexToMove + 1, rowIndex, isSelecting);

		//1.5. 노트와 페이징버퍼에서 너비만큼 이동한다.
		columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		row->Move(columnIndex);
		row->SelectRange(columnIndex, row->GetLength(), isSelecting);
		pagingBuffer->Next(columnIndex);

		//1.6. 줄 수가 충분치 않다면,
		if (restedRowCount > 0)
		{
			//1.5.1. 적재한다.
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();

			//1.5.2. 나머지 줄 수 만큼 내려간다.
			rowIndexToMove = rowIndex - restedRowCount;
			if (rowIndex < 0)
			{
				rowIndex = 0;
			}
			rowIndexToMove = note->Move(rowIndexToMove);
			row = note->GetAt(rowIndexToMove);
			row->First();
			pagingBuffer->PreviousRow(restedRowCount);
			note->SelectRange(rowIndexToMove + 1, rowIndex, isSelecting);
		}

		//1.7 줄에서 너비와 가장 가까운 위치로 이동한다.
		columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		row->Move(columnIndex);
		row->SelectRange(columnIndex, row->GetLength(), isSelecting);
		pagingBuffer->Next(columnIndex);

		//1.6. 적재범위를 벗어났으면 재적재한다.
		if (note->IsAboveTopLine(rowIndex))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
		}

		//1.7. 스크롤을 올린다.
		Long pos = vScroll.GetPos() - vScroll.GetPage();
		if (pos < 0)
		{
			pos = 0;
		}
		scrollController->MoveVScroll(pos);
	}
#endif
}