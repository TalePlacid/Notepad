#include <afxwin.h>
#include "ShiftRightAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftRightAction::ShiftRightAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftRightAction::~ShiftRightAction() {

}

void ShiftRightAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 끝이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset;
	if (columnIndex < row->GetLength())
	{
		//2.1. 노트에서 다음 단어 시작으로 이동한다.
		Long wordStart = row->FindNextWordStart(columnIndex);
		Long movedIndex = row->Move(wordStart);

		//2.2. 노트에서 범위만큼 선택한다.
		row->ToggleSelection(columnIndex, movedIndex);

		//2.3. 차이만큼 반복한다.
		Long difference = movedIndex - columnIndex;
		Long i = 0;
		while (i < difference)
		{
			//2.3.1. 페이징 버퍼에서 마킹되어 있지 않으면, 마킹한다.
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}

			//2.3.2. 페이징 버퍼에서 이동한다. 
			currentOffset = pagingBuffer->Next();

			//2.3.3. 마킹되어 있던 위치로 돌아왔으면, 마킹을 지운다.
			if (currentOffset == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}

			i++;
		}
	}
	else //3. 줄의 끝이면,
	{
		//3.1. 적재범위를 벗어나면, 재적재한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * sizeCalculator->GetRowHeight();
		if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}

		//3.2. 노트에서 이동한다.
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		//3.3. 줄이 진짜이면,
		if (!row->IsDummyRow())
		{
			//3.3.1. 페이징 버퍼에서 마킹되어 있지 않으면, 마킹한다.
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}

			//3.3.2. 페이징 버퍼에서 이동한다. 
			currentOffset = pagingBuffer->NextRow();

			//3.3.3. 마킹되어 있던 위치로 돌아왔으면, 마킹을 지운다.
			if (currentOffset == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
	}
#if 0
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex < row->GetLength())
	{
		Glyph* character = row->GetAt(columnIndex);
		if (!character->IsSelected())
		{
			character->Select(TRUE);
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}
			row->Next();
			pagingBuffer->Next();
		}
		else
		{
			character->Select(FALSE);
			row->Next();
			pagingBuffer->Next();
			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
	}
	else
	{
		if (note->IsBelowBottomLine(rowIndex+1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex < note->GetLength() - 1)
		{
			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}

			rowIndex = note->Next();
			pagingBuffer->NextRow();

			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();

			if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
			{
				pagingBuffer->UnmarkSelectionBegin();
			}
		}
	}
#endif
}