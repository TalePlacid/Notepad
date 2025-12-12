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
		//1.1. 내려갈 만큼의 줄 수를 구한다.
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

		//1.3. 줄수만큼, 전체 줄위치보다 작으면 반복한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long selectionBeginOffset;
		Long previousOffset = -1;
		Long currentOffset = pagingBuffer->GetCurrentOffset();

		Long previousRowIndex = -1;
		Long i = 1;
		while (i <= rowCount && rowIndex != previousRowIndex)
		{
			//1.3.1. 현재줄에서 선택한다.
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
				if (columnIndex < row->GetLength())
				{
					row->GetAt(columnIndex)->ToggleSelection();
				}

				previousOffset = currentOffset;
				currentOffset = pagingBuffer->Next();
				if (currentOffset == selectionBeginOffset)
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
				columnIndex = row->Next();
			}

			//1.3.2. 다음 줄이 적재범위를 넘어서면 재적재한다.
			if (note->IsBelowBottomLine(rowIndex + 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
				rowIndex = note->GetCurrent();
			}

			//1.3.3. 다음줄로 이동한다.
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->NextRow();
			previousRowIndex = rowIndex;
			rowIndex = note->Next();
			i++;

			//1.3.4. 다음줄이 있다면,
			if (previousRowIndex != rowIndex)
			{
				//1.3.3.2. 너비에 근접한 열까지 반복한다.
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
				Long nearestColumnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
				Long j = 0;
				while (previousOffset != currentOffset && j < nearestColumnIndex)
				{
					//1.3.3.2.1. 페이징 버퍼에서 마킹한다.
					selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
					if (selectionBeginOffset < 0)
					{
						pagingBuffer->MarkSelectionBegin();
					}
					else if (currentOffset == selectionBeginOffset)
					{
						pagingBuffer->UnmarkSelectionBegin();
					}

					//1.3.3.2.2. 노트에서 선택반전한다.
					if (columnIndex < row->GetLength())
					{
						row->GetAt(columnIndex)->ToggleSelection();
					}

					previousOffset = currentOffset;
					currentOffset = pagingBuffer->Next();
					if (currentOffset == selectionBeginOffset)
					{
						pagingBuffer->UnmarkSelectionBegin();
					}
					columnIndex = row->Next();
					j++;
				}
			}
		}
	}


#if 0
	//1. 수직 스크롤바가 존재하고, 마지막 위치가 아니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (scrollController->HasVScroll() && vScroll.GetPos() < posLimit)
	{
		//1.1. 내려갈 만큼의 줄 수를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long rowCount = vScroll.GetPage() / rowHeight;
		if (vScroll.GetPage() % rowHeight > 0)
		{
			rowCount++;
		}

		//1.2. 줄 수만큼, 마지막줄까지 반복한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();
		Long rowWidth = sizeCalculator->GetRowWidth(row, columnIndex);

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long nearestColumnIndex;
		Long j;
		Long previousRowIndex = -1;
		Long i = 1;
		while (i <= rowCount && rowIndex != previousRowIndex)
		{
			//1.2.1. 다음줄이 적재 범위 밖이면 재적재한다.
			if (note->IsBelowBottomLine(rowIndex + 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
				rowIndex = note->GetCurrent();
			}

			//1.2.2. 현재줄에서 이동, 선택한다.
			while (columnIndex < row->GetLength())
			{
				if (!row->GetAt(columnIndex)->IsSelected())
				{
					if (pagingBuffer->GetSelectionBeginOffset() < 0)
					{
						pagingBuffer->MarkSelectionBegin();
					}
					row->GetAt(columnIndex)->Select(true);
				}
				else
				{
					row->GetAt(columnIndex)->Select(false);
				} 

				columnIndex = row->Next();
				pagingBuffer->Next();
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}

			//1.2.3. 다음줄로 이동한다.
			previousRowIndex = rowIndex;
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			pagingBuffer->NextRow();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{          
				pagingBuffer->UnmarkSelectionBegin();
			}
			i++;

			//1.2.4. 다음줄에서 이동, 선택한다.
			nearestColumnIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
			while (columnIndex < nearestColumnIndex && columnIndex < row->GetLength())
			{
				if (!row->GetAt(columnIndex)->IsSelected())
				{
					if (pagingBuffer->GetSelectionBeginOffset() < 0)
					{
						pagingBuffer->MarkSelectionBegin();
					}
					row->GetAt(columnIndex)->Select(true);
				}
				else
				{
					row->GetAt(columnIndex)->Select(false);
				}

				columnIndex = row->Next();
				pagingBuffer->Next();
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
		}
	}
#endif
#if 0
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
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		MarkingHelper markingHelper(this->parent);
		markingHelper.Mark();
		bool isSelecting = true;
		if (pagingBuffer->GetCurrentOffset() < pagingBuffer->GetSelectionBeginOffset())
		{
			isSelecting = false;
		}
		row->SelectRange(columnIndex, row->GetLength(), isSelecting);
	
		//1.4. 노트와 페이징버퍼에서 줄 수 만큼 내려간다.
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
		note->SelectRange(rowIndex, rowIndexToMove - 1, isSelecting);

		//1.5. 노트와 페이징버퍼에서 너비만큼 이동하며 선택한다.
		columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
		columnIndex = row->Move(columnIndex);
		row->SelectRange(0, columnIndex, isSelecting);
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
			note->SelectRange(rowIndex, rowIndexToMove - 1, isSelecting);

			//1.6.4. 노트와 페이징버퍼에서 너비만큼 이동하며 선택한다.
			columnIndex = sizeCalculator->GetNearestColumnIndex(row, originalRowWidth);
			columnIndex = row->Move(columnIndex);
			row->SelectRange(0, columnIndex, isSelecting);
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
#endif
}