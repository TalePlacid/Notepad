#include <afxwin.h>
#include "SelectWordLeftAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

SelectWordLeftAction::SelectWordLeftAction(CWnd* parent)
	:Action(parent) {

}

SelectWordLeftAction::~SelectWordLeftAction() {

}

void SelectWordLeftAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 처음이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	if (columnIndex > 0)
	{
		//2.1. 단어 시작을 찾는다.
		Long wordStart = row->FindPreviousWordStart(columnIndex);

		//2.2. 단어 시작까지 반복한다.
		Long bytes;
		Long i = columnIndex;
		while (i > wordStart)
		{
			//2.2.1. 노트에서 이동하면 선택한다.
			columnIndex = row->Previous();
			row->GetAt(columnIndex)->ToggleSelection();
			bytes = row->GetAt(columnIndex)->GetBytes();

			//2.2.2. 페이징 버퍼에서 선택하며 이동한다.
			pagingBuffer->BeginSelectionIfNeeded();
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
			pagingBuffer->EndSelectionIfCollapsed();

			i--;
		}
	}
	else //3. 줄의 처음이면,
	{
		//3.1. 적재범위를 벗어났으면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			PageManager::LoadPrevious(this->parent);
			rowIndex = note->GetCurrent();
		}

		//3.2. 첫번째 줄이 아니면,
		if (rowIndex > 0)
		{
			//3.2.1. 노트에서 이동한다.
			row = note->GetAt(rowIndex);
			rowIndex = note->Previous();
			Glyph* movedRow = note->GetAt(rowIndex);
			columnIndex = movedRow->Last();

			//3.2.2. 원래줄이 진짜줄이면, 페이징버퍼에서 선택하며 이동한다.
			if (!row->IsDummyRow())
			{
				pagingBuffer->BeginSelectionIfNeeded();
				currentOffset = pagingBuffer->MoveOffset(currentOffset - 2);
				pagingBuffer->EndSelectionIfCollapsed();
			}
		}
	}
}
