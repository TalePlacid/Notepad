#include <afxwin.h>
#include "PagingNavigator.h"
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "resource.h"

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
			row->First();
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
			row->First();
		}

		currentOffset = pagingBuffer->PreviousRow();
	}

	while (currentOffset < offset)
	{
		currentOffset = pagingBuffer->Next();
		row->Next();
	}

	return pagingBuffer->GetCurrentOffset();
}