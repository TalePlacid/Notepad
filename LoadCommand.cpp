#include <afxwin.h>
#include "LoadCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "resource.h"
#include "ScrollController.h"
#include "GlyphFactory.h"

#pragma warning(disable:4996)

LoadCommand::LoadCommand(CWnd* parent)
	:Command(parent) {

}

LoadCommand::~LoadCommand() {

}

void LoadCommand::Execute() {
	// 1. 노트를 적재한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* loadedNote = pagingBuffer->LoadNext();

	// 2. 기존 노트가 있다면,
	if (note != NULL)
	{
		// 1.1.상단부만 남기고 지운다.
		Long rowIndex = note->GetCurrent();
		Long index = rowIndex - PAGE_ROWCOUNT;
		if (index < 0)
		{
			index = 0;
		}

		note->TruncateBefore(index);
		pagingBuffer->CacheRowStartIndex(index);
		
		// 1.2. 적재한 노트와 합친다.
		//note->Append(loadedNote);
		if (loadedNote != NULL)
		{
			delete loadedNote;
		}
	}
	else // 2. 노트가 없다면,
	{
		// 2.1. 적재한 노트를 노트로 삼는다.
		((NotepadForm*)(this->parent))->note = loadedNote;
	}

	// 4. 노트에 선택사항을 반영한다.
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

#if 0
	// 1. 페이징버퍼에서 로드한다.
	Long previousIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	
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

	//4. 스크롤바 차이를 기억해둔다.
	((NotepadForm*)(this->parent))->scrollController->RecordDifference(previousIndex, rowIndex);

	//5. 스크롤바를 업데이트한다.
	//((NotepadForm*)(this->parent))->Notify("UpdateScrollBars");
#endif
}