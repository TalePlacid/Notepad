#include <afxwin.h>
#include "MoveWordRightAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

MoveWordRightAction::MoveWordRightAction(CWnd* parent)
	:Action(parent) {

}

MoveWordRightAction::~MoveWordRightAction() {

}

void MoveWordRightAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 마지막이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	if (columnIndex < row->GetLength())
	{
		//2.1. 노트에서 다음 단어 시작으로 이동한다.
		Long wordStart = row->FindNextWordStart(columnIndex);
		row->Move(wordStart);

		//2.2. 페이징 버퍼에서 이동한다.
		Long bytes = 0;
		Long i = columnIndex;
		while (i < wordStart)
		{
			bytes += row->GetAt(i)->GetBytes();
			i++;
		}

		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}
	else //3. 줄의 마지막이면,
	{
		//3.1. 적재범위를 벗어나면, 재적재한다.
		if (!note->IsLastPage() && note->IsBelowBottomLine(rowIndex + 1))
		{
			PageManager::LoadNext(this->parent);
			rowIndex = note->GetCurrent();
		}

		//3.2. 마지막 줄이 아니면,
		if (rowIndex + 1 < note->GetLength())
		{
			//3.2.1. 노트에서 이동한다.
			rowIndex = note->Next();
			Glyph* nextRow = note->GetAt(rowIndex);
			columnIndex = nextRow->First();

			//3.2.2. 이동한 줄이 진짜 줄이면, 페이징버퍼에서 이동한다.
			if (!nextRow->IsDummyRow())
			{
				currentOffset = pagingBuffer->MoveOffset(currentOffset + 2);
			}
		}
	}
}
