#include <afxwin.h>
#include "MoveRightAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

MoveRightAction::MoveRightAction(CWnd* parent)
	:Action(parent) {

}

MoveRightAction::~MoveRightAction() {

}

void MoveRightAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//1. 줄의 끝이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long bytes;
	if (columnIndex < row->GetLength())
	{
		bytes = row->GetAt(columnIndex)->GetBytes();
		columnIndex = row->Next();
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}
	else
	{
		if (!note->IsLastPage() && note->IsBelowBottomLine(rowIndex + 1))
		{
			PageManager::LoadNext(this->parent);
			rowIndex = note->GetCurrent();
		}

		if (rowIndex < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			Glyph* nextRow = note->GetAt(rowIndex);
			columnIndex = nextRow->First();

			if (!nextRow->IsDummyRow())
			{
				currentOffset = pagingBuffer->MoveOffset(currentOffset + 2);
			}
		}
	}
}
