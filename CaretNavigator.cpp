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
	while (currentOffset < offset && rowIndex < note->GetLength() - 1)
	{
		if (!note->IsLastPage() && note->IsBelowBottomLine(rowIndex + 1))
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

	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long bytes = currentRow->GetPreviousBytes(currentColumnIndex);
	currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
	while (currentRowIndex > targetRowIndex)
	{
		previousRow = currentRow;
		currentRowIndex = note->Previous();
		currentRow = note->GetAt(currentRowIndex);
		currentColumnIndex = currentRow->Last();
		if (!previousRow->IsDummyRow())
		{
			currentOffset = pagingBuffer->MoveOffset(currentOffset - 2);
		}

		currentRow->First();
		bytes = currentRow->GetBytes();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
	}

	Long nearestIndex = ((NotepadForm*)(this->parent))->sizeCalculator->GetNearestColumnIndex(currentRowIndex, rowWidth);
	currentRow->Move(nearestIndex);
	bytes = currentRow->GetPreviousBytes(nearestIndex);
	currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
}

void CaretNavigator::MoveCaretDownToAbsoluteRow(Long absoluteRowIndex, Long rowWidth) {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	Long rowEndIndex = rowStartIndex + note->GetLength();
	Long previousRowEndIndex = -1;
	while (absoluteRowIndex >= rowEndIndex && previousRowEndIndex != rowEndIndex && !note->IsLastPage())
	{
		previousRowEndIndex = rowStartIndex + note->GetLength();
		PageManager::LoadNext(this->parent);
		note = ((NotepadForm*)(this->parent))->note;
		pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		rowStartIndex = pagingBuffer->GetRowStartIndex();
		rowEndIndex = rowStartIndex + note->GetLength();
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

	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long bytes = currentRow->GetPreviousBytes(currentColumnIndex);
	currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
	while (currentRowIndex < targetRowIndex)
	{
		previousRow = currentRow;
		currentRowIndex = note->Next();
		currentRow = note->GetAt(currentRowIndex);
		currentRow->First();
		bytes = previousRow->GetBytes();
		if (!currentRow->IsDummyRow())
		{
			bytes += 2;
		}
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}

	Long nearestIndex = ((NotepadForm*)(this->parent))->sizeCalculator->GetNearestColumnIndex(currentRowIndex, rowWidth);
	currentRow->Move(nearestIndex);
	bytes = currentRow->GetPreviousBytes(nearestIndex);
	currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
}

void CaretNavigator::AdjustCaretUpToVScroll() {
	//1. 스크롤에 해당하는 절대줄위치를 구한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long pos = vScroll.GetPos();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();

	Long absoluteAboveRowIndex = pos / rowHeight;
	if (pos % rowHeight > 0)
	{
		absoluteAboveRowIndex++;
	}

	//2. 이동해야할 줄 수를 구한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();

	Long rowCount = absoluteAboveRowIndex - (rowIndex + rowStartIndex);

	//3. 줄수가 부족하다면 적재한다.
	if (note->IsBelowBottomLine(rowIndex + rowCount) && !note->IsLastPage())
	{
		PageManager::LoadNext(this->parent);
		rowIndex = note->GetCurrent();
	}

	//4. 노트와 페이징버퍼에서 이동한다.
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(rowIndex, columnIndex);
	Long nearestIndex;
	Long bytes;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	while (rowCount > 0 && rowIndex < note->GetLength() - 1)
	{
		rowIndex = note->Next();
		bytes = row->GetNextBytes(columnIndex);
		row = note->GetAt(rowIndex);
		if (!row->IsDummyRow())
		{
			bytes += 2;
		}
		nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, rowWidth);
		columnIndex = row->Move(nearestIndex);
		bytes += row->GetPreviousBytes(columnIndex);

		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);

		rowCount--;
	}
}

void CaretNavigator::AdjustCaretDownToVScroll() {
	//1. 스크롤 끝에 해당하는 절대줄위치를 구한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long endPos = vScroll.GetPos() + vScroll.GetPage();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();

	Long absoluteBelowRowIndex = endPos / rowHeight - 1;
	if (absoluteBelowRowIndex < 0)
	{
		absoluteBelowRowIndex = 0;
	}

	//2. 이동해야할 줄 수를 구한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();

	Long rowCount = (rowIndex + rowStartIndex) - absoluteBelowRowIndex;

	//3. 줄수가 부족하다면 적재한다.
	if (note->IsAboveTopLine(rowIndex - rowCount) && rowStartIndex > 0)
	{
		PageManager::LoadPrevious(this->parent);
		rowIndex = note->GetCurrent();
	}

	//4. 노트와 페이징버퍼에서 이동한다.
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	Long rowWidth = sizeCalculator->GetRowWidth(rowIndex, columnIndex);
	Long nearestIndex;
	Long bytes;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	while (rowCount > 0 && rowIndex > 0)
	{
		rowIndex = note->Previous();
		bytes = row->GetPreviousBytes(columnIndex);
		if (!row->IsDummyRow())
		{
			bytes += 2;
		}
		row = note->GetAt(rowIndex);
		nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, rowWidth);
		columnIndex = row->Move(nearestIndex);
		bytes += row->GetNextBytes(columnIndex);

		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

		rowCount--;
	}
}

