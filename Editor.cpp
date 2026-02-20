#include <afxwin.h>
#include "Editor.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "CaretNavigator.h"
#include "glyphs/Glyph.h"
#include "ByteChecker.h"
#include "NoteWrapper.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PageLoader.h"
#include "glyphs/GlyphFactory.h"
#include "RowCounter.h"

#pragma warning(disable:4996)

Editor::Editor(CWnd* parent) {
	this->parent = parent;
}

Editor::~Editor() {

}

void Editor::InsertTextAt(Long offset, Long columnIndex, CString text, bool isSelected) {
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(offset);
	caretNavigator.NormalizeColumn(columnIndex);

	//2. 현재위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();

	//3. 복사할 내용의 끝까지 반복한다.
	GlyphFactory glyphFactory;
	ByteChecker byteChecker;
	NoteWrapper noteWrapper(this->parent);
	Glyph* glyph;
	TCHAR character[2];
	Long dummied = 0;
	Long i = 0;
	while (i < text.GetLength())
	{
		//3.1. 문자를 읽는다.
		character[0] = text.GetAt(i);
		if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
		{
			character[1] = text.GetAt(++i);
		}

		//3.2. 줄바꿈 문자가 아니라면, 줄에서 쓴다.
		if (character[0] != '\r')
		{
			glyph = glyphFactory.Create(character, isSelected);
			row->Add(currentColumnIndex, glyph);
			currentColumnIndex = row->GetCurrent();
		}
		else //3.3. 줄바꿈 문자라면, 줄을 나눈다.
		{
			note->SplitRows(currentRowIndex, currentColumnIndex);
			currentRowIndex = note->Next();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->First();
		}

		//3.4. 자동개행중이면, 재개행한다.
		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			dummied += noteWrapper.Rewrap();
			currentRowIndex = note->GetCurrent();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->GetCurrent();
		}
		i++;
	}

	//4. 페이징 버퍼에서 쓴다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (isSelected)
	{
		pagingBuffer->MarkSelectionBegin();
	}
	pagingBuffer->Add(text);

	//5. 수직 스크롤바에서 최대값을 조정한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + (RowCounter::CountRow(text) + dummied) * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}
}

void Editor::EraseRange(Long frontOffset, Long rearOffset, Long& columnIndex, CString& contents) {
	//1. 앞 위치로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(frontOffset);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	columnIndex = currentColumnIndex;

	//3. 선택 길이만큼 반복한다.
	ByteChecker byteChecker;
	Long merged = 0;
	Glyph* nextRow;
	TCHAR* character;
	Long characterByte;
	Long selectionLength = rearOffset - frontOffset;
	Long i = 0;
	BOOL flag = TRUE;
	while (i < selectionLength && flag)
	{
		flag = FALSE;
		//3.1. 줄의 끝이 아니라면,
		if (currentColumnIndex < row->GetLength())
		{
			flag = TRUE;
			//3.1.1. 구한다.
			character = (char*)*row->GetAt(currentColumnIndex);
			characterByte = 1;
			if (byteChecker.IsLeadByte(character[0]))
			{
				characterByte = 2;
			}

			//3.1.2. 노트에서 지운다.
			row->Remove(currentColumnIndex);
			row->Move(currentColumnIndex);
		}
		else if (currentRowIndex + 1 < note->GetLength()) //3.2. 줄의 끝이고 다음 줄이 있다면,
		{
			flag = TRUE;
			//3.2.1. 바이트수를 구한다.
			nextRow = note->GetAt(currentRowIndex + 1);
			if (!nextRow->IsDummyRow())
			{
				characterByte = 2;
			}
			else
			{
				characterByte = 0;
			}

			//3.2.2. 줄을 합친다.
			note->MergeRows(currentRowIndex);
			merged++;
		}

		i += characterByte;
	}

	//4. 자동개행중이면, 재개행한다.
	Long dummied = 0;
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		NoteWrapper noteWrapper(this->parent);
		dummied = noteWrapper.Rewrap();
	}

	//5. 페이징버퍼에서 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Remove(rearOffset);
	pagingBuffer->UnmarkSelectionBegin();

	//6. 수직 스크롤을 조정한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	Scroll vScroll = scrollController->GetVScroll();
	if (scrollController->HasVScroll())
	{
		Long max = vScroll.GetMax() - (merged + dummied) * rowHeight;
		scrollController->ResizeVRange(max);
	}

	//7. 적재량이 부족하면, 재적재한다.
	Long pageMax = vScroll.GetPos() + vScroll.GetPage();
	if (note->IsBelowBottomLine(currentRowIndex + 1) && pageMax < vScroll.GetMax())
	{
		row = note->GetAt(currentRowIndex);
		currentColumnIndex = row->GetCurrent();
		if (!row->IsDummyRow() && currentColumnIndex == 0)
		{
			pagingBuffer->Add(CString("\r\n"));
			pagingBuffer->PreviousRow();
		}
		PageLoader::LoadNext(this->parent);
		if (!row->IsDummyRow() && columnIndex == 0)
		{
			note->MergeRows(currentRowIndex);
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
		}
	}
}

bool Editor::GetSelectedRange(Long& frontOffset, Long& rearOffset) {
	bool ret = false;

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	if (selectionBeginOffset >= 0)
	{
		ret = true;

		Long currentOffset = pagingBuffer->GetCurrentOffset();
		frontOffset = currentOffset;
		rearOffset = selectionBeginOffset;
		if (currentOffset > selectionBeginOffset)
		{
			frontOffset = selectionBeginOffset;
			rearOffset = currentOffset;
		}
	}

	return ret;
}
