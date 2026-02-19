#include <afxwin.h>
#include "MoveWordLeftAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

MoveWordLeftAction::MoveWordLeftAction(CWnd* parent)
	:Action(parent) {

}

MoveWordLeftAction::~MoveWordLeftAction() {

}

void MoveWordLeftAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 처음이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		//2.1. 노트에서 이전 단어 시작으로 이동한다.
		Long wordStart = row->FindPreviousWordStart(columnIndex);
		Long movedIndex = row->Move(wordStart);
		
		//2.2. 페이징 버퍼에서 이동한다.
		pagingBuffer->Previous(columnIndex - movedIndex);
	}
	else //3. 줄의 처음이면,
	{
		//3.1. 적재 범위를 벗어나면, 재적재한다.
		if (note->IsBelowBottomLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			PageLoader::LoadPrevious(this->parent);
			rowIndex = note->GetCurrent();
		}

		//3.2. 첫번째 줄이 아니라면,
		if (rowIndex > 0)
		{
			//3.2.1. 노트에서 이동한다.
			rowIndex = note->Previous();
			Glyph* previousRow = note->GetAt(rowIndex);
			columnIndex = previousRow->Last();

			//3.2.2. 원래 줄이 진짜 줄이면, 페이징 버퍼에서 이동한다.
			if (!row->IsDummyRow())
			{
				pagingBuffer->PreviousRow();
				pagingBuffer->Last();
			}
		}
	}
}