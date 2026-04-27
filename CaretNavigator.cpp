#include <afxwin.h>
#include "CaretNavigator.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "NoteWrapper.h"
#include "PageManager.h"

#pragma warning(disable:4996)

CaretNavigator::CaretNavigator(CWnd* parent) {
	this->parent = parent;
}

CaretNavigator::~CaretNavigator() {

}

Long CaretNavigator::MoveTo(Long offset) {
	//1. 현재 줄의 첫 위치로 이동한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long bytes = row->GetPreviousBytes(columnIndex);
	columnIndex = row->First();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

	//2. 현재 오프셋이 지정 offset보다 이전이면 반복한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll;
	Long pageMax;
	while (currentOffset < offset && rowIndex < note->GetLength() - 1)
	{
		vScroll = scrollController->GetVScroll();
		pageMax = vScroll.GetPos() + vScroll.GetPage();
		if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
		{
			PageManager::LoadNext(this->parent);
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
		}

		bytes = row->GetBytes();
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		if (!row->IsDummyRow())
		{
			bytes += 2;
		}

		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}

	//3. 현재 offset이 지정 offset보다 이후이면 반복한다.
	while (currentOffset > offset)
	{
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			PageManager::LoadPrevious(this->parent);
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
		}

		bytes = 2;
		if (row->IsDummyRow())
		{
			bytes = 0;
		}
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		bytes += row->GetBytes();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
	}

	//4. 지정 offset과 같아질때까지 반복한다.
	while (currentOffset < offset)
	{
		bytes = row->GetAt(columnIndex)->GetBytes();
		columnIndex = row->Next();
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}

	return pagingBuffer->GetCurrentOffset();
}

void CaretNavigator::NormalizeColumn(Long columnIndex) {
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long currentRowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(currentRowIndex);
		Long currentColumnIndex = row->GetCurrent();
		Long lastColumnIndex = row->GetLength();

		if (lastColumnIndex > 0)
		{
			if (columnIndex == 0 && currentColumnIndex == lastColumnIndex)
			{
				currentRowIndex = note->Next();
				row = note->GetAt(currentRowIndex);
				row->First();
			}
			else if (columnIndex == lastColumnIndex && currentColumnIndex == 0)
			{
				currentRowIndex = note->Previous();
				row = note->GetAt(currentRowIndex);
				row->Last();
			}
		}
	}
}

void CaretNavigator::MoveCaretUpToAbsoluteRow(Long absoluteRowIndex, Long rowWidth) {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long previousRowStartIndex = -1;
	while (absoluteRowIndex < rowStartIndex && rowStartIndex > 0 && previousRowStartIndex != rowStartIndex)
	{
		previousRowStartIndex = rowStartIndex;
		PageManager::LoadPrevious(this->parent);
		note = ((NotepadForm*)(this->parent))->note;
		pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		rowStartIndex = pagingBuffer->GetRowStartIndex();
	}

	Long targetRowIndex = absoluteRowIndex - rowStartIndex;
	if (targetRowIndex < 0)
	{
		targetRowIndex = 0;
	}
	else if (targetRowIndex >= note->GetLength())
	{
		targetRowIndex = note->GetLength() - 1;
	}

	Glyph* previousRow;
	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();
	currentRow->First();
	pagingBuffer->Previous(currentColumnIndex);
	while (currentRowIndex > targetRowIndex)
	{
		previousRow = currentRow;
		currentRowIndex = note->Previous();
		currentRow = note->GetAt(currentRowIndex);
		currentColumnIndex = currentRow->Last();
		if (!previousRow->IsDummyRow())
		{
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
		}
		currentRow->First();
		pagingBuffer->Previous(currentRow->GetLength());
	}

	Long nearestIndex = ((NotepadForm*)(this->parent))->sizeCalculator->GetNearestColumnIndex(currentRowIndex, rowWidth);
	currentRow->Move(nearestIndex);
	pagingBuffer->Next(nearestIndex);
}

void CaretNavigator::MoveCaretDownToAbsoluteRow(Long absoluteRowIndex, Long rowWidth) {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long previousRowStartIndex = -1;
	while (absoluteRowIndex >= rowStartIndex + note->GetLength() && previousRowStartIndex != rowStartIndex)
	{
		previousRowStartIndex = rowStartIndex;
		PageManager::LoadNext(this->parent);
		note = ((NotepadForm*)(this->parent))->note;
		pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		rowStartIndex = pagingBuffer->GetRowStartIndex();
	}

	Long targetRowIndex = absoluteRowIndex - rowStartIndex;
	if (targetRowIndex < 0)
	{
		targetRowIndex = 0;
	}
	else if (targetRowIndex >= note->GetLength())
	{
		targetRowIndex = note->GetLength() - 1;
	}

	Glyph* previousRow;
	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();
	currentRow->First();
	pagingBuffer->Previous(currentColumnIndex);
	while (currentRowIndex < targetRowIndex)
	{
		previousRow = currentRow;
		currentRowIndex = note->Next();
		currentRow = note->GetAt(currentRowIndex);
		currentRow->First();
		if (currentRow->IsDummyRow())
		{
			pagingBuffer->Next(previousRow->GetLength());
		}
		else
		{
			pagingBuffer->NextRow();
		}
	}

	Long nearestIndex = ((NotepadForm*)(this->parent))->sizeCalculator->GetNearestColumnIndex(currentRowIndex, rowWidth);
	currentRow->Move(nearestIndex);
	pagingBuffer->Next(nearestIndex);
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

	Long rowCount = rowIndexToMove - rowIndex;

	//3. 적재범위를 벗어나면, 재적재한다.
	Long pageMax = (rowStartIndex + note->GetLength()) * rowHeight;
	if (note->IsBelowBottomLine(rowIndexToMove) && pageMax < vScroll.GetMax())
	{
		PageManager::LoadNext(this->parent);
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
		nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, rowWidth);
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

	Long rowCount = rowIndex - rowIndexToMove;

	//3. 적재범위를 넘어섰으면, 재적재한다.
	if (note->IsAboveTopLine(rowIndexToMove) && rowStartIndex > 0)
	{
		PageManager::LoadPrevious(this->parent);
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
		nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, rowWidth);
		while (columnIndex > nearestIndex)
		{
			columnIndex = row->Previous();
			pagingBuffer->Previous();
		}

		i++;
	}
}

