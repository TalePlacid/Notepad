#include <afxwin.h>
#include "PageManager.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "glyphs/Glyph.h"
#include "glyphs/Note.h"
#include "glyphs/NoteWidthCache.h"
#include "glyphs/RowWidthCache.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "NoteWrapper.h"
#include "SuspendAutoWrap.h"
#include "NoteConverter.h"

#pragma warning(disable:4996)

PageManager::PageManager() {

}

PageManager::~PageManager() {

}

void PageManager::LoadFirst(CWnd* parent) {
	SuspendAutoWrap suspendAutoWrap(parent);

	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	pagingBuffer->FirstRow();

	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadNext(loadedContents, loadedByteCount);

	NoteConverter noteConverter;
	Glyph* loadedNote = noteConverter.Convert(loadedContents);

	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(-rowStartIndex);

	if (((NotepadForm*)parent)->note != NULL)
	{
		delete ((NotepadForm*)parent)->note;
	}
	((NotepadForm*)parent)->note = loadedNote;

	if (((NotepadForm*)parent)->noteWidthCache != NULL)
	{
		((NotepadForm*)parent)->Unregister(((NotepadForm*)parent)->noteWidthCache);
		delete ((NotepadForm*)parent)->noteWidthCache;
	}
	((NotepadForm*)parent)->noteWidthCache = new NoteWidthCache(parent, ((NotepadForm*)parent)->note);
	((NotepadForm*)parent)->Register(((NotepadForm*)parent)->noteWidthCache);

	Glyph* note = ((NotepadForm*)parent)->note;
	Long rowIndex = note->First();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->First();

	Long i = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long previousRowIndex = -1;
	while (rowIndex != previousRowIndex && i < selectionBeginOffset)
	{
		while (columnIndex < row->GetLength() && i < selectionBeginOffset)
		{
			row->GetAt(columnIndex)->Select(true);
			columnIndex = row->Next();
			i = pagingBuffer->Next();
		}

		previousRowIndex = rowIndex;
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		i = pagingBuffer->NextRow();
	}

	rowIndex = note->First();
	row = note->GetAt(rowIndex);
	columnIndex = row->First();
	pagingBuffer->FirstRow();

	TRACE("LoadFirst\n");
}

void PageManager::LoadPrevious(CWnd* parent) {
	//자동개행시 임시 개행 풀기
	SuspendAutoWrap suspendAutoWrap(parent);

	//1. 노트에서 현재 위치를 기억한다.
	Glyph* note = ((NotepadForm*)parent)->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	Long noteLength = note->GetLength();

	//2. 노트의 2번째 줄 기준으로 적재 위치를 맞춘다.
	Long rowIndex = 0;
	if (noteLength > 1)
	{
		rowIndex = 1;
	}

	rowIndex = note->Move(rowIndex);
	if (currentRowIndex > rowIndex)
	{
		pagingBuffer->PreviousRow(currentRowIndex - rowIndex);
	}
	else if (currentRowIndex < rowIndex)
	{
		pagingBuffer->NextRow(rowIndex - currentRowIndex);
	}

	row = note->GetAt(rowIndex);
	Long columnIndex = row->First();
	pagingBuffer->First();
	Long baseOffset = pagingBuffer->GetCurrentOffset();

	//3. 앞 부분을 로드한다.
	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadPrevious(loadedContents, loadedByteCount);
	Glyph* loadedNote = NULL;
	NoteWidthCache* loadedCache = NULL;
	if (loadedByteCount > 0)
	{
		NoteConverter noteConverter;
		loadedNote = noteConverter.Convert(loadedContents);
		loadedCache = new NoteWidthCache(parent, loadedNote);
	}

	//4. 실제 적재 바이트가 있을 때만 기존 첫 줄을 교체한다.
	NoteWidthCache* noteWidthCache = ((NotepadForm*)parent)->noteWidthCache;
	Long count = 0;
	if (loadedByteCount > 0 && loadedNote != NULL)
	{
		if (noteLength > 1)
		{
			note->Remove(0);
			noteWidthCache->Remove(0);
		}

		count = note->AppendFromFront(loadedNote);
		noteWidthCache->AppendFromFront(*loadedCache);
		Long rowStartDifference = -count;
		if (noteLength > 1)
		{
			rowStartDifference++;
		}
		pagingBuffer->CacheRowStartIndex(rowStartDifference);

		//4.1. 선택여부를 반영한다.
		Long previousRowIndex = -1;
		rowIndex = count;
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
		Long i = pagingBuffer->GetCurrentOffset();
		while (rowIndex >= 0 && previousRowIndex != rowIndex && selectionBeginOffset >= 0 && i > selectionBeginOffset)
		{
			while (columnIndex > 0 && i > selectionBeginOffset)
			{
				row->GetAt(columnIndex - 1)->Select(true);
				columnIndex = row->Previous();
				i = pagingBuffer->Previous();
			}

			previousRowIndex = rowIndex;
			Long nextRowIndex = note->Previous();
			if (nextRowIndex < previousRowIndex)
			{
				rowIndex = nextRowIndex;
				row = note->GetAt(rowIndex);
				columnIndex = row->Last();
				i = pagingBuffer->PreviousRow();
				i = pagingBuffer->Last();
			}
			else
			{
				rowIndex = nextRowIndex;
			}
		}
	}

	if (loadedCache != NULL)
	{
		delete loadedCache;
	}

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	//5. 로드 결과에 맞춰 현재 줄 위치를 보정한다.
	if (loadedByteCount > 0 && loadedNote != NULL)
	{
		rowIndex = count;
		currentRowIndex += count;
		if (noteLength > 1)
		{
			currentRowIndex--;
		}
	}

	if (currentRowIndex < 0)
	{
		currentRowIndex = 0;
	}
	else if (currentRowIndex >= note->GetLength())
	{
		currentRowIndex = note->GetLength() - 1;
	}

	rowIndex = note->Move(rowIndex);
	row = note->GetAt(rowIndex);
	row->First();
	pagingBuffer->MoveOffset(baseOffset);

	Glyph* previousRow = NULL;
	while (rowIndex > currentRowIndex)
	{
		previousRow = row;
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		row->Last();

		if (!previousRow->IsDummyRow())
		{
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
		}

		row->First();
		pagingBuffer->Previous(row->GetLength());
	}

	while (rowIndex < currentRowIndex)
	{
		previousRow = row;
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		row->First();

		if (row->IsDummyRow())
		{
			pagingBuffer->Next(previousRow->GetLength());
		}
		else
		{
			pagingBuffer->NextRow();
		}
	}

	if (currentColumnIndex < 0)
	{
		currentColumnIndex = 0;
	}
	else if (currentColumnIndex > row->GetLength())
	{
		currentColumnIndex = row->GetLength();
	}

	row->Move(currentColumnIndex);
	pagingBuffer->Next(currentColumnIndex);
	currentRowIndex = note->GetCurrent();

	//6. 노트에서 아랫 부분을 지운다.
	Long belowIndex = currentRowIndex + PAGE_ROWCOUNT;
	note->TruncateAfter(belowIndex);
	noteWidthCache->TruncateAfter(belowIndex);

	//7. 수평 스크롤바 최대값을 갱신한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
	Long rowWidth = 0;
	Long max = scrollController->GetHScroll().GetMax();
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		rowWidth = sizeCalculator->GetRowWidth(i, row->GetLength());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		i++;
	}
	scrollController->ResizeHRange(max);
	
	TRACE("LoadPrevious\n");
}

void PageManager::LoadNext(CWnd* parent) {
	//자동개행시 임시 개행 풀기
	SuspendAutoWrap suspendAutoWrap(parent);

	//1. 노트에서 현재 위치를 기억한다.
	Glyph* note = ((NotepadForm*)parent)->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	//2. 마지막보다 1줄 이전 줄 기준으로 적재 위치를 맞춘다.
	Long rowIndex = note->Last();
	rowIndex = note->Previous();

	if (currentRowIndex > rowIndex)
	{
		pagingBuffer->PreviousRow(currentRowIndex - rowIndex);
	}
	else if (currentRowIndex < rowIndex)
	{
		pagingBuffer->NextRow(rowIndex - currentRowIndex);
	}

	row = note->GetAt(rowIndex);
	Long columnIndex = row->Last();
	Long lastOffset = pagingBuffer->Last();

	//3. 뒷 부분을 로드한다.
	if (lastOffset > 0 && lastOffset < pagingBuffer->GetFileEndOffset())
	{
		pagingBuffer->NextRow();
	}

	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadNext(loadedContents, loadedByteCount);
	Glyph* loadedNote = NULL;
	NoteWidthCache* loadedCache = NULL;
	if (loadedByteCount > 0)
	{
		NoteConverter noteConverter;
		loadedNote = noteConverter.Convert(loadedContents);
		loadedCache = new NoteWidthCache(parent, loadedNote);
	}

	//4. 실제 적재 바이트가 있을 때,
	if (loadedByteCount > 0 && loadedNote != NULL)
	{
		//4.1. 적재된 노트가 1줄 이상일 때(의미가 있는 노트일 때)
		NoteWidthCache* noteWidthCache = ((NotepadForm*)parent)->noteWidthCache;
		if (loadedNote->GetLength() > 1)
		{
			//4.1.1. 중첩되는 1줄을 지운다.
			note->Remove(note->GetLength() - 1);
			noteWidthCache->Remove(noteWidthCache->GetLength() - 1);

			//4.1.2. 적재된 노트를 붙인다.
			note->AppendFromRear(loadedNote);
			noteWidthCache->AppendFromRear(*loadedCache);

			//4.1.3. 로드된 뒷 부분의 선택여부를 반영한다.
			Long previousRowIndex = -1;
			Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
			rowIndex = note->GetLength() - loadedNote->GetLength();
			if (rowIndex < 0)
			{
				rowIndex = 0;
			}
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			Long i = pagingBuffer->GetCurrentOffset();
			while (rowIndex < note->GetLength() && previousRowIndex != rowIndex && i < selectionBeginOffset)
			{
				while (columnIndex < row->GetLength() && i < selectionBeginOffset)
				{
					row->GetAt(columnIndex)->Select(true);
					columnIndex = row->Next();
					i = pagingBuffer->Next();
				}

				previousRowIndex = rowIndex;
				Long nextRowIndex = note->Next();
				if (nextRowIndex > previousRowIndex)
				{
					rowIndex = nextRowIndex;
					row = note->GetAt(rowIndex);
					columnIndex = row->First();
					i = pagingBuffer->NextRow();
				}
				else
				{
					rowIndex = nextRowIndex;
				}
			}

			//4.1.4. 노트에서 윗 부분을 지운다.
			Long upperIndex = currentRowIndex - PAGE_ROWCOUNT;
			if (upperIndex < 0)
			{
				upperIndex = 0;
			}
			note->TruncateBefore(upperIndex);
			noteWidthCache->TruncateBefore(upperIndex);
			pagingBuffer->CacheRowStartIndex(upperIndex);
			currentRowIndex -= upperIndex;

			//4.1.5. 수평 스크롤바 최대값을 갱신한다.
			Long rowWidth = 0;
			ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
			SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
			Long max = scrollController->GetHScroll().GetMax();
			i = 0;
			while (i < note->GetLength())
			{
				row = note->GetAt(i);
				rowWidth = sizeCalculator->GetRowWidth(i, row->GetLength());
				if (rowWidth > max)
				{
					max = rowWidth;
				}
				i++;
			}
			scrollController->ResizeHRange(max);
		}
	}

	if (loadedCache != NULL)
	{
		delete loadedCache;
	}

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	//5. 현재 위치를 직접 복원한다.
	rowIndex = note->Move(currentRowIndex);
	row = note->GetAt(rowIndex);
	currentColumnIndex = row->Move(currentColumnIndex);
	pagingBuffer->MoveOffset(currentOffset);
	
	TRACE("LoadNext\n");
}

void PageManager::LoadLast(CWnd* parent) {
	//자동개행 일시 정지
	SuspendAutoWrap suspendAutoWrap(parent);

	//1. 페이징버퍼에서 마지막위치로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	pagingBuffer->LastRow();
	pagingBuffer->Last();

	//2. 페이징버퍼에서 한 줄을 추가한다.
	TCHAR character[2];
	character[0] = '\r';
	character[1] = '\n';
	Long last = pagingBuffer->Add(character);

	//3. 페이징 버퍼에서 이전 부분을 적재한다.
	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadPrevious(loadedContents, loadedByteCount);

	NoteConverter noteConverter;
	Glyph* loadedNote = noteConverter.Convert(loadedContents);

	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	//4. 페이지버퍼에서 추가했던 줄을 지운다.
	pagingBuffer->MoveOffset(last);
	pagingBuffer->Remove();

	//5. 페이징버퍼에서 캐싱한다.
	Long fileEndOffset = pagingBuffer->GetFileEndOffset();
	Long totalRowCount = pagingBuffer->CountRow(fileEndOffset);
	Long difference = totalRowCount - loadedNote->GetLength() - pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(difference);

	//6. 노트를 교체한다.
	if (((NotepadForm*)parent)->note != NULL)
	{
		delete ((NotepadForm*)parent)->note;
	}
	((NotepadForm*)parent)->note = loadedNote;

	if (((NotepadForm*)parent)->noteWidthCache != NULL)
	{
		((NotepadForm*)parent)->Unregister(((NotepadForm*)parent)->noteWidthCache);
		delete ((NotepadForm*)parent)->noteWidthCache;
	}
	((NotepadForm*)parent)->noteWidthCache = new NoteWidthCache(parent, ((NotepadForm*)parent)->note);
	((NotepadForm*)parent)->Register(((NotepadForm*)parent)->noteWidthCache);

	//7. 노트에서 마지막으로 이동한다.
	Glyph* note = ((NotepadForm*)parent)->note;
	Long rowIndex = note->Last();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Last();

	//8. 선택여부를 반영한다.
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long offset = pagingBuffer->GetCurrentOffset();
	Long previousColumnIndex;
	Long previousRowIndex = -1;
	while (rowIndex != previousRowIndex && selectionBeginOffset >= 0 && offset >= selectionBeginOffset)
	{
		previousColumnIndex = columnIndex;
		columnIndex = row->Previous();
		offset = pagingBuffer->Previous();
		while (columnIndex != previousColumnIndex && offset >= selectionBeginOffset)
		{
			row->GetAt(columnIndex)->Select(true);
			previousColumnIndex = columnIndex;
			columnIndex = row->Previous();
			offset = pagingBuffer->Previous();
		}

		previousRowIndex = rowIndex;
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();
		offset = pagingBuffer->PreviousRow();
		offset = pagingBuffer->Last();
	}

	//9. 노트에서 마지막으로 이동한다.
	rowIndex = note->Last();
	row = note->GetAt(rowIndex);
	columnIndex = row->Last();
	pagingBuffer->MoveOffset(pagingBuffer->GetFileEndOffset());

	TRACE("LoadLast\n");
}

void PageManager::ReloadAfterErase(CWnd* parent) {
	//1. 자동개행을 일시정지한다.
	SuspendAutoWrap suspendAutoWrap(parent);

	//2. 현재 노트와 위치를 읽는다.
	Glyph* note = ((NotepadForm*)parent)->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* currentRow = note->GetAt(currentRowIndex);
	Long currentColumnIndex = currentRow->GetCurrent();

	//3. 현재 줄 아래의 기존 적재분은 버린다.
	NoteWidthCache* noteWidthCache = ((NotepadForm*)parent)->noteWidthCache;
	currentRow->TruncateAfter(currentColumnIndex);
	noteWidthCache->GetAt(currentRowIndex)->Recalculate(currentRow);
	note->TruncateAfter(currentRowIndex);
	noteWidthCache->TruncateAfter(currentRowIndex);

	//4. 현재 offset의 뒷부분을 다시 읽는다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadNext(loadedContents, loadedByteCount);

	Glyph* loadedNote = NULL;
	NoteWidthCache* loadedCache = NULL;
	if (loadedByteCount > 0)
	{
		NoteConverter noteConverter;
		loadedNote = noteConverter.Convert(loadedContents);
		loadedCache = new NoteWidthCache(parent, loadedNote);
	}

	//5. 다시 읽은 첫 줄은 현재 줄 뒤에 붙인다.
	if (loadedNote != NULL)
	{
		if (loadedNote->GetLength() > 0)
		{
			Glyph* firstLoadedRow = loadedNote->GetAt(0);
			Long i = 0;
			while (i < firstLoadedRow->GetLength())
			{
				currentRow->Add(firstLoadedRow->GetAt(i)->Clone());
				noteWidthCache->GetAt(currentRowIndex)->Recalculate(currentRow);
				i++;
			}
		}

		//5.1. 첫 줄을 제외한 나머지 줄들은 노트 뒤에 붙인다.
		if (loadedNote->GetLength() > 0)
		{
			loadedNote->Remove(0);
			loadedCache->Remove(0);
		}

		if (loadedNote->GetLength() > 0)
		{
			note->AppendFromRear(loadedNote);
			noteWidthCache->AppendFromRear(*loadedCache);
		}
	}

	if (loadedCache != NULL)
	{
		delete loadedCache;
	}

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	//6. 현재 위치를 복원한다.
	currentRow->Move(currentColumnIndex);

	//7. 수평 스크롤 최대값을 다시 계산한다.
	ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
	Long max = 0;
	Long rowWidth = 0;
	Long j = 0;
	while (j < note->GetLength())
	{
		Glyph* row = note->GetAt(j);
		rowWidth = sizeCalculator->GetRowWidth(j, row->GetLength());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		j++;
	}
	scrollController->ResizeHRange(max);

	TRACE("ReloadAfterErase\n");
}

void PageManager::TrimIfNeeded(CWnd* parent) {
	SuspendAutoWrap suspendAutoWrap(parent);

	Glyph* note = ((NotepadForm*)parent)->note;
	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	Long noteLength = note->GetLength();
	Long maxRowCount = PAGE_ROWCOUNT * 5;
	if (noteLength > maxRowCount)
	{
		Long currentRowIndex = note->GetCurrent();
		Long aboveRowIndex = currentRowIndex - PAGE_ROWCOUNT * 2;
		Long aboveRest = 0;
		if (aboveRowIndex < 0)
		{
			aboveRest = -aboveRowIndex;
			aboveRowIndex = 0;
		}

		Long belowRowIndex = currentRowIndex + PAGE_ROWCOUNT * 2;
		Long belowRest = 0;
		if (belowRowIndex > noteLength - 1)
		{
			belowRest = belowRowIndex - (noteLength - 1);
			belowRowIndex = noteLength - 1;
		}

		belowRowIndex += aboveRest;
		aboveRowIndex -= belowRest;

		NoteWidthCache* noteWidthCache = ((NotepadForm*)parent)->noteWidthCache;
		note->TruncateAfter(belowRowIndex);
		noteWidthCache->TruncateAfter(belowRowIndex);
		note->TruncateBefore(aboveRowIndex);
		noteWidthCache->TruncateBefore(aboveRowIndex);
		pagingBuffer->CacheRowStartIndex(aboveRowIndex);

		TRACE("TRIM: %ld(%ld), %ld(%ld)\n", note->GetCurrent(), note->GetCurrent() + pagingBuffer->GetRowStartIndex(),
			note->GetLength(), note->GetLength() + pagingBuffer->GetRowStartIndex());
	}
}







