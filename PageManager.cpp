#include <afxwin.h>
#include "PageManager.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "glyphs/Glyph.h"
#include "glyphs/Note.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "NoteWrapper.h"
#include "SuspendAutoWrap.h"
#include "CaretNavigator.h"
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
	Long currentOffset = pagingBuffer->GetCurrentOffset();
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
	//3. 앞 부분을 로드한다.
	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadPrevious(loadedContents, loadedByteCount);
	Glyph* loadedNote = NULL;
	if (loadedByteCount > 0)
	{
		NoteConverter noteConverter;
		loadedNote = noteConverter.Convert(loadedContents);
	}

	//4. 실제 적재 바이트가 있을 때만 기존 첫 줄을 교체한다.
	Long count = 0;
	if (loadedByteCount > 0 && loadedNote != NULL)
	{
		if (noteLength > 1)
		{
			note->Remove(0);
		}

		count = note->AppendFromFront(loadedNote);
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

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	//5. 현재 위치로 돌아온다.
	CaretNavigator caretNavigator(parent);
	caretNavigator.MoveTo(currentOffset);
	note = ((NotepadForm*)parent)->note;
	currentRowIndex = note->GetCurrent();
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->GetCurrent();


	//6. 노트에서 아랫 부분을 지운다.
	Long belowIndex = currentRowIndex + PAGE_ROWCOUNT;
	note->TruncateAfter(belowIndex);

	//7. 수평 스크롤바 최대값을 갱신한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
	Long rowWidth = 0;
	Long max = scrollController->GetHScroll().GetMax();
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		rowWidth = sizeCalculator->GetRowWidth(row, row->GetLength());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		i++;
	}
	scrollController->ResizeHRange(max);
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
	Long noteLength = note->GetLength();

	//2. 마지막보다 1줄 이전 줄 기준으로 적재 위치를 맞춘다.
	Long rowIndex = note->GetLength() - 1;
	if (noteLength > 1)
	{
		rowIndex--;
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
	Long columnIndex = row->Last();
	Long lastOffset = pagingBuffer->Last();

	//3. 뒷 부분을 로드한다.
	if (lastOffset > 0 && lastOffset < pagingBuffer->GetFileEndOffset())
	{
		pagingBuffer->NextRow();
		pagingBuffer->First();
	}

	TCHAR* loadedContents = NULL;
	Long loadedByteCount = 0;
	pagingBuffer->LoadNext(loadedContents, loadedByteCount);
	Glyph* loadedNote = NULL;
	if (loadedByteCount > 0)
	{
		NoteConverter noteConverter;
		loadedNote = noteConverter.Convert(loadedContents);
	}

	//4. 실제 적재 바이트가 있을 때만 기존 마지막 줄을 교체한다.
	Long count = 0;
	if (loadedByteCount > 0 && loadedNote != NULL)
	{
		if (noteLength > 0)
		{
			note->Remove(note->GetLength() - 1);
		}

		count = note->AppendFromRear(loadedNote);

		//4.1. 로드된 뒷 부분의 선택여부를 반영한다.
		Long previousRowIndex = -1;
		Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
		rowIndex = note->GetLength() - count;
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
	}

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}
	if (loadedContents != NULL)
	{
		delete[] loadedContents;
	}

	//5. 현재 위치로 돌아온다.
	CaretNavigator caretNavigator(parent);
	caretNavigator.MoveTo(currentOffset);
	note = ((NotepadForm*)parent)->note;
	currentRowIndex = note->GetCurrent();
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->GetCurrent();

	//6. 노트에서 윗 부분을 지운다.
	Long upperIndex = currentRowIndex - PAGE_ROWCOUNT;
	if (upperIndex < 0)
	{
		upperIndex = 0;
	}
	note->TruncateBefore(upperIndex);
	pagingBuffer->CacheRowStartIndex(upperIndex);

	//7. 수평 스크롤바 최대값을 갱신한다.
	Long rowWidth = 0;
	ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
	Long max = scrollController->GetHScroll().GetMax();
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		rowWidth = sizeCalculator->GetRowWidth(row, row->GetLength());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		i++;
	}
	scrollController->ResizeHRange(max);
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
}

void PageManager::TrimIfNeeded(CWnd* parent) {
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

		note->TruncateAfter(belowRowIndex);
		note->TruncateBefore(aboveRowIndex);
		pagingBuffer->CacheRowStartIndex(aboveRowIndex);

	}
}







