#include <afxwin.h>
#include "PagingNavigator.h"
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "resource.h"
#include "ScrollController.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

PagingNavigator::PagingNavigator(CWnd* parent) {
	this->parent = parent;
}

PagingNavigator::~PagingNavigator() {

}

Long PagingNavigator::MoveTo(Long offset) {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->First();
	Long currentOffset = pagingBuffer->First();

	if (currentOffset > offset)
	{
		while (currentOffset <= offset)
		{
			if (note->IsAboveTopLine(rowIndex - 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			}
			rowIndex = note->Previous();
			currentOffset = pagingBuffer->PreviousRow();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}
	}
	else if (currentOffset > offset)
	{
		while (currentOffset >= offset)
		{
			if (note->IsBelowBottomLine(rowIndex + 1))
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			}
			rowIndex = note->Next();
			currentOffset = pagingBuffer->NextRow();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}

		currentOffset = pagingBuffer->PreviousRow();
	}

	while (currentOffset < offset)
	{
		currentOffset = pagingBuffer->Next();
		columnIndex = row->Next();
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;

	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long rowStartIndex = pagingBuffer->GetRowStartIndex();
		Long height = (rowStartIndex + rowIndex) * rowHeight;
		if (height + rowHeight < vScroll.GetPos() || height > vScroll.GetPos() + vScroll.GetPage())
		{
			Long pos = height + rowHeight * 2 - vScroll.GetPage();
			scrollController->MoveVScroll(pos);
		}
	}

	if (scrollController->HasHScroll())
	{
		Scroll hScroll = scrollController->GetHScroll();
		Long width = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			width += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i)));
			i++;
		}

		Long multiCharacterWidth = sizeCalculator->GetMultiByteWidth();
		if (width < hScroll.GetPos() || width + multiCharacterWidth > hScroll.GetPos() + hScroll.GetPage())
		{
			Long pos = width + multiCharacterWidth * 2 - hScroll.GetPage();
			scrollController->MoveHScroll(pos);
		}
	}

	return pagingBuffer->GetCurrentOffset();
}