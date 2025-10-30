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
	// 1. ��Ʈ�� �����Ѵ�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* loadedNote = pagingBuffer->LoadNext();

	// 2. ���� ��Ʈ�� �ִٸ�,
	if (note != NULL)
	{
		// 1.1.��ܺθ� ����� �����.
		Long rowIndex = note->GetCurrent();
		Long index = rowIndex - PAGE_ROWCOUNT;
		if (index < 0)
		{
			index = 0;
		}

		note->TruncateBefore(index);
		pagingBuffer->CacheRowStartIndex(index);
		
		// 1.2. ������ ��Ʈ�� ��ģ��.
		note->Append(loadedNote);
		if (loadedNote != NULL)
		{
			delete loadedNote;
		}
	}
	else // 2. ��Ʈ�� ���ٸ�,
	{
		// 2.1. ������ ��Ʈ�� ��Ʈ�� ��´�.
		((NotepadForm*)(this->parent))->note = loadedNote;
	}

	// 4. ��Ʈ�� ���û����� �ݿ��Ѵ�.
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

#if 0
	// 1. ����¡���ۿ��� �ε��Ѵ�.
	Long previousIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	((NotepadForm*)(this->parent))->note = pagingBuffer->Load();
	Position current = pagingBuffer->GetCurrent();

	// 2. ��Ʈ�� ���� ��ġ�� �����Ѵ�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(current.GetColumn());

	// 3. ��Ʈ�� ���û����� �ݿ��Ѵ�.
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

	//4. ��ũ�ѹ� ���̸� ����صд�.
	((NotepadForm*)(this->parent))->scrollController->RecordDifference(previousIndex, rowIndex);

	//5. ��ũ�ѹٸ� ������Ʈ�Ѵ�.
	//((NotepadForm*)(this->parent))->Notify("UpdateScrollBars");
#endif
}