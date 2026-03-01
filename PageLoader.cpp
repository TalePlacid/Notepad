#include <afxwin.h>
#include "PageLoader.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "glyphs/Glyph.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "NoteWrapper.h"
#include "SuspendAutoWrap.h"

#pragma warning(disable:4996)

PageLoader::PageLoader() {

}

PageLoader::~PageLoader() {

}

void PageLoader::LoadFirst(CWnd* parent) {
	SuspendAutoWrap suspendAutoWrap(parent);

	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	pagingBuffer->FirstRow();

	Glyph* loadedNote = pagingBuffer->LoadNext();

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

void PageLoader::LoadPrevious(CWnd* parent) {
	//자동개행시 임시 개행 풀기
	SuspendAutoWrap suspendAutoWrap(parent);

	//1. 노트에서 현재 위치를 기억한다.
	Glyph* note = ((NotepadForm*)parent)->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	//2. 노트의 시작점으로 이동한다.
	Long rowIndex = note->First();
	pagingBuffer->PreviousRow(currentRowIndex - rowIndex);
	row = note->GetAt(rowIndex);
	Long columnIndex = row->First();
	pagingBuffer->First();

	//3. 앞 부분을 로드한다.
	Glyph* loadedNote = pagingBuffer->LoadPrevious();

	//4. 로드된 분량을 붙인다.
	Long count = note->AppendFromFront(loadedNote);
	pagingBuffer->CacheRowStartIndex(-count);

	//5. 선택여부를 반영한다.
	Long previousRowIndex = -1;
	rowIndex += count;
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
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Last();
		i = pagingBuffer->PreviousRow();
		i = pagingBuffer->Last();
	}

	//6. 현재 위치로 돌아온다.
	currentRowIndex = note->Move(currentRowIndex + count);
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->Move(currentColumnIndex);
	pagingBuffer->MoveOffset(currentOffset);

	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	//7. 노트에서 아랫 부분을 지운다.
	Long belowIndex = currentRowIndex + PAGE_ROWCOUNT;
	note->TruncateAfter(belowIndex);

	//8. 수평 스크롤바 최대값을 갱신한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
	ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
	Long rowWidth = 0;
	Long max = scrollController->GetHScroll().GetMax();
	i = 0;
	while (i < note->GetLength())
	{
		rowWidth = sizeCalculator->GetRowWidth(note->GetAt(i)->MakeString().c_str());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		i++;
	}
	scrollController->ResizeHRange(max);
}

void PageLoader::LoadNext(CWnd* parent) {
	//자동개행시 임시 개행 풀기
	SuspendAutoWrap suspendAutoWrap(parent);

	//1. 노트에서 현재 위치를 기억한다.
	Glyph* note = ((NotepadForm*)parent)->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	//2. 노트의 마지막 위치로 이동한다.
	Long rowIndex = note->Move(note->GetLength() - 1);
	pagingBuffer->NextRow(rowIndex - currentRowIndex);
	row = note->GetAt(rowIndex);
	Long columnIndex = row->Last();
	Long lastOffset = pagingBuffer->Last();

	//3. 뒷 부분을 로드한다.
	if (lastOffset > 0 && lastOffset < pagingBuffer->GetFileEndOffset())
	{
		pagingBuffer->NextRow();
		pagingBuffer->First();
	}
	Glyph* loadedNote = pagingBuffer->LoadNext();

	//4. 로드된 뒷 부분을 기존의 분량에 합친다.
	note->AppendFromRear(loadedNote);
	if (loadedNote != NULL)
	{
		delete loadedNote;
	}

	//5. 로드된 뒷 부분의 선택여부를 반영한다.
	Long previousRowIndex = -1;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	rowIndex = note->Next();
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
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		i = pagingBuffer->NextRow();
	}

	//6. 현재 위치로 돌아온다.
	currentRowIndex = note->Move(currentRowIndex);
	row = note->GetAt(currentRowIndex);
	currentColumnIndex = row->Move(currentColumnIndex);
	pagingBuffer->MoveOffset(currentOffset);

	//7. 노트에서 윗 부분을 지운다.
	Long upperIndex = currentRowIndex - PAGE_ROWCOUNT;
	if (upperIndex < 0)
	{
		upperIndex = 0;
	}
	note->TruncateBefore(upperIndex);
	pagingBuffer->CacheRowStartIndex(upperIndex);

	//8. 수평 스크롤바 최대값을 갱신한다.
	Long rowWidth = 0;
	ScrollController* scrollController = ((NotepadForm*)parent)->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)parent)->sizeCalculator;
	Long max = scrollController->GetHScroll().GetMax();
	i = 0;
	while (i < note->GetLength())
	{
		rowWidth = sizeCalculator->GetRowWidth(note->GetAt(i)->MakeString().c_str());
		if (rowWidth > max)
		{
			max = rowWidth;
		}
		i++;
	}
	scrollController->ResizeHRange(max);
}

void PageLoader::LoadLast(CWnd* parent) {
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
	Glyph* loadedNote = pagingBuffer->LoadPrevious();

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
