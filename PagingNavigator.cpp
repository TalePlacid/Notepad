#include <afxwin.h>
#include "PagingNavigator.h"
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "resource.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

PagingNavigator::PagingNavigator(CWnd* parent) {
	this->parent = parent;
}

PagingNavigator::~PagingNavigator() {

}

Long PagingNavigator::MoveTo(Long offset) {
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
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
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
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
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

void PagingNavigator::NormalizeColumn(Long columnIndex) {
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long currentRowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(currentRowIndex);
		Long currentColumnIndex = row->GetCurrent();

		if (columnIndex < currentColumnIndex)
		{
			currentRowIndex = note->Next();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->First();
		}
		else if (columnIndex > currentColumnIndex)
		{
			currentRowIndex = note->Previous();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->Last();
		}
	}
}