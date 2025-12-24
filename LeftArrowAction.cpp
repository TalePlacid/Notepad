#include <afxwin.h>
#include "LeftArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

LeftArrowAction::LeftArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

LeftArrowAction::~LeftArrowAction() {

}

void LeftArrowAction::Perform() {
	//1. 페이징 버퍼에서 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long previousOffset = pagingBuffer->GetCurrentOffset();
	Long currentOffset = pagingBuffer->Previous();
	if (previousOffset == currentOffset)
	{
		currentOffset = pagingBuffer->PreviousRow();
		currentOffset = pagingBuffer->Last();
	}

	//2. 페이징 버퍼에서 이동했다면,
	if (previousOffset != currentOffset)
	{
		//2.1. 노트에서 이동한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		if (columnIndex > 0)
		{
			columnIndex = row->Previous();
		}
		else
		{
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
				rowIndex = note->GetCurrent();
			}

			if (rowIndex > 0)
			{
				rowIndex = note->Previous();
				row = note->GetAt(rowIndex);
				columnIndex = row->Last();
			}
		}
	}
}