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
	NoteWrapper noteWrapper(this->parent);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	//1. 줄의 처음으로 이동한다.
	Long columnIndex = row->First();
	Long currentOffset = pagingBuffer->First();

	//2. 목표가 위이면,
	Long previousOffset = -1;
	if (currentOffset > offset)
	{
		//2.1. 목표보다 크고, 이전위치랑 현재위치가 다르면, 반복한다.
		while (currentOffset > offset && previousOffset != currentOffset)
		{
			//2.1.1. 이전 줄이 적재범위에서 벗어나면 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			}

			//2.1.2. 이전 줄로 이동한다.
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->PreviousRow();
		}
	}
	else if (currentOffset < offset) // 3. 목표가 아래이면,
	{
		// 3.1. 목표보다 작고, 이전 위치와 현재 위치가 다르면, 반복한다.
		while (currentOffset <= offset && previousOffset != currentOffset)
		{
			// 3.1.1. 아랫줄이 적재범위를 넘어서면, 재적재한다.
			if (note->IsBelowBottomLine(rowIndex + 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			}

			// 3.1.2. 다음 줄로 이동한다.
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->NextRow();
		}

		// 3.2. 이전 위치와 현재 위치가 다르면, 이전 줄로 이동한다.
		if (previousOffset != currentOffset)
		{
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			previousOffset = currentOffset;
			currentOffset = pagingBuffer->PreviousRow();
		}
	}

	// 4. 목표와 같아질 때까지 반복한다.
	while (currentOffset < offset)
	{
		// 4.1. 다음으로 이동한다.
		columnIndex = row->Next();
		currentOffset = pagingBuffer->Next();
	}

	return pagingBuffer->GetCurrentOffset();
}