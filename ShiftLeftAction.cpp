#include <afxwin.h>
#include "ShiftLeftAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftLeftAction::ShiftLeftAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftLeftAction::~ShiftLeftAction() {

}

void ShiftLeftAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 처음이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset;
	if (columnIndex > 0)
	{
		//2.1. 노트에서 이동한다.
		columnIndex = row->Previous();

		//2.2. 노트에서 선택한다.
		row->GetAt(columnIndex)->ToggleSelection();

		//2.3. 페이징 버퍼에서 선택 시작 위치가 없으면, 마킹한다.
		if (pagingBuffer->GetSelectionBeginOffset() < 0)
		{
			pagingBuffer->MarkSelectionBegin();
		}

		//2.4. 페이징 버퍼에서 이동한다.
		currentOffset = pagingBuffer->Previous();

		//2.5. 페이징 버퍼에서 선택시작위치로 돌아왔으면, 마킹 해제한다.
		if (currentOffset == pagingBuffer->GetSelectionBeginOffset())
		{
			pagingBuffer->UnmarkSelectionBegin();
		}
	}
	else //3. 줄의 처음이면,
	{
		//3.1. 적재범위를 벗어났으면 적재한다.
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		//3.2. 첫번째 줄이 아니면,
		if (rowIndex > 0)
		{
			//3.2.1. 노트에서 이동한다.
			row = note->GetAt(rowIndex);
			rowIndex = note->Previous();
			Glyph* movedRow = note->GetAt(rowIndex);
			columnIndex = row->Last();

			//3.2.2. 원래 줄이 진짜 줄이면,
			if (!row->IsDummyRow())
			{
				//3.2.2.1. 페이징 버퍼에서 선택되지 않았다면 마킹한다.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//3.2.2.2. 페이징 버퍼에서 이동한다.
				currentOffset = pagingBuffer->PreviousRow();
				currentOffset = pagingBuffer->Last();

				//3.2.2.3. 마킹 위치로 돌아왔다면 마킹해제한다.
				if (currentOffset == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
		}		
	}
}