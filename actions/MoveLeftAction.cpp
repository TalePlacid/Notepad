#include <afxwin.h>
#include "MoveLeftAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

MoveLeftAction::MoveLeftAction(CWnd* parent)
	:Action(parent) {

}

MoveLeftAction::~MoveLeftAction() {

}

void MoveLeftAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 처음이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (columnIndex > 0)
	{
		columnIndex = row->Previous();
		pagingBuffer->Previous();
	}
	else //3. 줄의 처음이면,
	{
		//3.1. 적재범위를 넘어서면 적재한다.
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			PageLoader::LoadPrevious(this->parent);
			rowIndex = note->GetCurrent();
		}

		//3.2. 첫번째 줄이 아니면,
		if (rowIndex > 0)
		{
			//3.2.1. 노트에서 이동한다.
			rowIndex = note->Previous();
			Glyph* previousRow = note->GetAt(rowIndex);
			columnIndex = previousRow->Last();

			//3.2.2. 원래 줄이 더미 줄이 아니면,
			if (!row->IsDummyRow())
			{
				pagingBuffer->PreviousRow();
				pagingBuffer->Last();
			}
		}
	}
}