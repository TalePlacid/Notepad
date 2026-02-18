#include <afxwin.h>
#include "CaretNavigator.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "NoteWrapper.h"
#include "PageLoader.h"

#pragma warning(disable:4996)

CaretNavigator::CaretNavigator(CWnd* parent) {
	this->parent = parent;
}

CaretNavigator::~CaretNavigator() {

}

Long CaretNavigator::MoveTo(Long offset) {
	//1. 줄의 처음으로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->First();

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	while (row->IsDummyRow())
	{
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
	}
	Long columnIndex = row->First();

	//2. 목표가 위이면,
	Long previousOffset = -1;
	if (currentOffset > offset)
	{
		//2.1. 목표보다 크고, 이전위치랑 현재위치가 다르면, 반복한다.
		while (currentOffset > offset && previousOffset != currentOffset)
		{
			//2.1.1. 이전 줄이 적재범위에서 벗어나면 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageLoader::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
			}

			//2.1.2. 이전 줄로 이동한다.
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->PreviousRow();

			if (previousOffset != currentOffset)
			{
				rowIndex = note->Previous();
				row = note->GetAt(rowIndex);
				while (row->IsDummyRow())
				{
					rowIndex = note->Previous();
					row = note->GetAt(rowIndex);
				}
				columnIndex = row->First();
			}
		}
	}
	else if (currentOffset < offset) // 3. 목표가 아래이면,
	{
		// 3.1. 목표보다 작고, 이전 위치와 현재 위치가 다르면, 반복한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		while (currentOffset <= offset && previousOffset != currentOffset)
		{
			// 3.1.1. 아랫줄이 적재범위를 넘어서면, 재적재한다.
			Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * sizeCalculator->GetRowHeight();
			if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
			{
				PageLoader::LoadNext(this->parent);
				rowIndex = note->GetCurrent();
			}

			// 3.1.2. 다음 줄로 이동한다.
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->NextRow();

			if (previousOffset != currentOffset)
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				while (row->IsDummyRow())
				{
					rowIndex = note->Next();
					row = note->GetAt(rowIndex);
				}
				columnIndex = row->First();
			}
		}

		// 3.2. 이전 위치와 현재 위치가 다르면, 이전 줄로 이동한다.
		if (previousOffset != currentOffset)
		{
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->PreviousRow();

			if (previousOffset != currentOffset)
			{
				rowIndex = note->Previous();
				row = note->GetAt(rowIndex);
				while (row->IsDummyRow())
				{
					rowIndex = note->Previous();
					row = note->GetAt(rowIndex);
				}
				columnIndex = row->First();
			}
		}
	}

	// 4. 목표와 같아질 때까지 반복한다.
	while (currentOffset < offset)
	{
		// 4.1. 다음으로 이동한다.
		currentOffset = pagingBuffer->Next();

		if (columnIndex < row->GetLength())
		{
			columnIndex = row->Next();
		}
		else
		{
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			if (row->IsDummyRow())
			{
				columnIndex = row->Next();
			}
		}
	}

	return pagingBuffer->GetCurrentOffset();
}

void CaretNavigator::NormalizeColumn(Long columnIndex) {
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long currentRowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(currentRowIndex);
		Long currentColumnIndex = row->GetCurrent();
		Long lastColumnIndex = row->GetLength();

		if (columnIndex == 0 && currentColumnIndex == lastColumnIndex)
		{
			currentRowIndex = note->Next();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->First();
		}
		else if (columnIndex == lastColumnIndex && currentColumnIndex == 0)
		{
			currentRowIndex = note->Previous();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->Last();
		}
	}
}

void CaretNavigator::AdjustCaretUpToVScroll(Long rowWidth) {
	//1. 스크롤에 해당하는 줄 위치를 구한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();

	Long rowIndexToMove = pos / rowHeight - rowStartIndex;
	if (pos % rowHeight > 0)
	{
		rowIndexToMove++;
	}

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 적재범위를 벗어나면, 재적재한다.
	Long pageMax = (rowStartIndex + note->GetLength()) * rowHeight;
	if (note->IsBelowBottomLine(rowIndexToMove) && pageMax < vScroll.GetMax())
	{
		PageLoader::LoadNext(this->parent);
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		rowStartIndex = pagingBuffer->GetRowStartIndex();
		rowIndexToMove = pos / rowHeight - rowStartIndex;
		if (pos % rowHeight > 0)
		{
			rowIndexToMove++;
		}
	}

	//4. 줄 수 만큼 반복한다.
	Long nearestIndex;
	Long rowCount = rowIndexToMove - rowIndex;
	Long i = 0;
	while (i < rowCount && rowIndex + 1 < note->GetLength())
	{
		//4.1. 줄의 끝까지 반복한다.
		while (columnIndex < row->GetLength())
		{
			columnIndex = row->Next();
			pagingBuffer->Next();
		}

		//4.2. 다음줄로 이동한다.
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		if (!row->IsDummyRow())
		{
			pagingBuffer->NextRow();
		}

		//4.3. 가까운 위치까지 반복한다.
		nearestIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
		while (columnIndex < nearestIndex)
		{
			columnIndex = row->Next();
			pagingBuffer->Next();
		}
		i++;
	}
}

void CaretNavigator::AdjustCaretDownToVScroll(Long rowWidth) {
	//1. 화면 끝에 해당하는 줄 위치를 찾는다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos();
	Long page = vScroll.GetPage();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();

	Long rowIndexToMove = (pos + page) / rowHeight - rowStartIndex - 1;
	if (rowIndexToMove < 0)
	{
		rowIndexToMove = 0;
	}

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 적재범위를 넘어섰으면, 재적재한다.
	if (note->IsAboveTopLine(rowIndexToMove) && rowStartIndex > 0)
	{
		PageLoader::LoadPrevious(this->parent);
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		rowStartIndex = pagingBuffer->GetRowStartIndex();
		rowIndexToMove = (pos + page) / rowHeight - rowStartIndex - 1;
		if (rowIndexToMove < 0)
		{
			rowIndexToMove = 0;
		}
	}

	//4. 줄 수 만큼 반복한다.
	Long nearestIndex;
	Glyph* previousRow;
	Long rowCount = rowIndex - rowIndexToMove;
	Long i = 0;
	while (i < rowCount && rowIndex > 0)
	{
		//4.1. 줄의 처음까지 반복한다.
		row = note->GetAt(rowIndex);
		while (columnIndex > 0)
		{
			columnIndex = row->Previous();
			pagingBuffer->Previous();
		}

		//4.2. 윗 줄로 이동한다.
		previousRow = row;
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();

		if (!previousRow->IsDummyRow())
		{
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
		}

		//4.3. 가까운 위치까지 반복한다.
		nearestIndex = sizeCalculator->GetNearestColumnIndex(row, rowWidth);
		while (columnIndex > nearestIndex)
		{
			columnIndex = row->Previous();
			pagingBuffer->Previous();
		}

		i++;
	}
}