#include <afxwin.h>
#include "LoadCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "resource.h"

#pragma warning(disable:4996)

LoadCommand::LoadCommand(CWnd* parent)
	:Command(parent) {

}

LoadCommand::~LoadCommand() {

}

void LoadCommand::Execute() {
	// 1. 페이징버퍼에서 로드한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	((NotepadForm*)(this->parent))->note = pagingBuffer->Load();
	Position current = pagingBuffer->GetCurrent();

	// 2. 노트의 현재 위치를 갱신한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(current.GetColumn());

	// 3. 노트의 선택사항을 반영한다.
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	if (selectionBeginOffset >= 0)
	{
		Long i;
		if (selectionBeginOffset < pagingBuffer->GetStartOffset())
		{
			note->SelectRange(pagingBuffer->GetStart(), pagingBuffer->GetCurrent());
		}
		else if (selectionBeginOffset > pagingBuffer->GetEndOffset())
		{
			note->SelectRange(pagingBuffer->GetCurrent(), pagingBuffer->GetEnd());
		}
		else
		{
			Long currentOffset = pagingBuffer->GetCurrentOffset();
			pagingBuffer->MoveOffset(selectionBeginOffset);
			Position selectionBegin = pagingBuffer->GetCurrent();
			pagingBuffer->MoveOffset(currentOffset);

			if (selectionBeginOffset <= currentOffset)
			{
				note->SelectRange(selectionBegin, pagingBuffer->GetCurrent());
			}
			else
			{
				note->SelectRange(pagingBuffer->GetCurrent(), selectionBegin);
			}
		}
	}
}