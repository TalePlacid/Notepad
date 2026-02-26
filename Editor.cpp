#include "Editor.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
#include "SearchResultController.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "CaretNavigator.h"
#include "NoteWrapper.h"
#include "PageLoader.h"
#include "ByteChecker.h"
#include "RowCounter.h"
#include "glyphs/GlyphFactory.h"

#pragma warning(disable:4996)

Editor::Editor(CWnd* parent) {
	this->parent = parent;
}

Editor::~Editor() {

}

void Editor::InsertTextAt(Long offset, Long columnIndex, CString text, BOOL isSelected) {
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
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	contents = pagingBuffer->MakeSelectedString();

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

void Editor::Replace(Long offset, CString sourceText, CString replacingText, 
	BOOL isSelected, Long& columnIndex) {
	//1. 검색된 위치로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(offset);
	caretNavigator.NormalizeColumn(columnIndex);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	columnIndex = currentColumnIndex;

	//3. 공통길이만큼 교체한다.
	Long sourceLength = RowCounter::CountCharacters(sourceText);
	Long replacedLength = RowCounter::CountCharacters(replacingText);
	Long commonLength;
	if (sourceLength < replacedLength)
	{
		commonLength = sourceLength;
	}
	else
	{
		commonLength = replacedLength;
	}

	ByteChecker byteChecker;
	TCHAR character[2];
	GlyphFactory glyphFactory;
	Glyph* glyph;
	Long j = 0;
	Long i = 0;
	while (i < commonLength)
	{
		if (currentColumnIndex < row->GetLength())
		{
			character[0] = replacingText.GetAt(j);
			if (byteChecker.IsLeadByte(character[0]))
			{
				character[1] = replacingText.GetAt(++j);
			}

			glyph = glyphFactory.Create(character, true);
			row->Replace(currentColumnIndex, glyph);
			currentColumnIndex = row->Next();

			j++;
		}

		if (currentColumnIndex >= row->GetLength())
		{
			currentRowIndex = note->Next();
			row = note->GetAt(currentRowIndex);
			currentColumnIndex = row->First();
		}

		i++;
	}

	//4. 바꾼 문자열이 더 길면, 노트에서 남는 길이만큼 추가한다.
	if (replacedLength > commonLength)
	{
		while (j < replacingText.GetLength())
		{
			character[0] = replacingText.GetAt(j);
			if (byteChecker.IsLeadByte(character[0]))
			{
				character[1] = replacingText.GetAt(++j);
			}

			glyph = glyphFactory.Create(character, true);
			row->Add(currentColumnIndex, glyph);
			currentColumnIndex = row->GetCurrent();

			j++;
		}
	}
	else if (sourceLength > commonLength) //5. 원본 문자열이 더 길면, 노트에서 남는 길이만큼 삭제한다.
	{
		Long restLength = sourceLength - commonLength;
		i = 0;
		while (i < restLength)
		{
			if (currentColumnIndex < row->GetLength())
			{
				row->Remove(currentColumnIndex);
				currentColumnIndex = row->Move(currentColumnIndex);
			}

			if (currentColumnIndex >= row->GetLength())
			{
				note->MergeRows(currentRowIndex);
			}

			i++;
		}
	}

	//6. 자동개행중이면, 재개행한다.
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		NoteWrapper noteWrapper(this->parent);
		noteWrapper.Rewrap();
	}

	//7. 페이징 버퍼에서 수정한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	if (sourceText.GetLength() <= replacingText.GetLength())
	{
		pagingBuffer->Replace(currentOffset, replacingText.Left(sourceText.GetLength()));
		pagingBuffer->Add(replacingText.Right(replacingText.GetLength() - sourceText.GetLength()));
	}
	else
	{
		pagingBuffer->Replace(currentOffset, replacingText);
		pagingBuffer->Remove(currentOffset + sourceText.GetLength());
	}
}

Long Editor::Find(FindReplaceOption findReplaceOption) {
	//1. 검색옵션을 최신화한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption option = searchResultController->ChangeFindReplaceOption(findReplaceOption);

	//2. 기존의 검색결과들을 지운다.
	searchResultController->Clear();

	//3. 검색한다.
	Long ret = searchResultController->Search();

	//4. 검색 결과가 있으면,
	Long nearestIndex = -1;
	if (searchResultController->GetLength() > 0)
	{
		//4.1. 가장 가까운 위치를 찾는다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		if (option.isSearchDown)
		{
			nearestIndex = searchResultController->FindNearestIndexBelow(currentOffset);
		}
		else
		{
			nearestIndex = searchResultController->FindNearestIndexAbove(currentOffset);
		}

		//4.2. 선택을 해제한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();

		//4.3. 위치로 이동한다.
		searchResultController->Move(nearestIndex);
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(searchResultController->GetAt(nearestIndex));
		caretNavigator.NormalizeColumn(0);

		//4.4. 선택한다.
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long characterLength;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < option.findString.GetLength())
		{
			characterLength = 0;
			if (columnIndex < row->GetLength())
			{
				characterLength = 1;
				character = row->GetAt(columnIndex);
				if (byteChecker.IsLeadByte(*(char*)*character))
				{
					characterLength = 2;
				}

				row->GetAt(columnIndex)->Select(true);
				columnIndex = row->Next();

				pagingBuffer->BeginSelectionIfNeeded();
				pagingBuffer->Next();
			}
			else
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}

			i += characterLength;
		}
	}

	//3. 검색결과가 없으면, 경고문을 출력한다.
	if (searchResultController->GetLength() <= 0 || nearestIndex < 0)
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)option.findString);
		this->parent->MessageBox(message);
	}

	return ret;
}

bool Editor::FindPrevious() {
	//1. 검색결과 컨트롤러에서 검색방향에 반대로 이동한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption option = searchResultController->GetFindReplaceOption();
	Long current = searchResultController->GetCurrent();
	Long previous = current;
	if (option.isSearchDown)
	{
		current = searchResultController->Previous();
	}
	else
	{
		current = searchResultController->Next();
	}

	//2. 선택해제한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 이동했으면,
	bool ret = false;
	if (current != previous)
	{
		ret = true;
		//2.1. 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(searchResultController->GetAt(current));
		caretNavigator.NormalizeColumn(0);

		//2.2. 선택한다.
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long characterLength;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < option.findString.GetLength())
		{
			characterLength = 0;
			if (columnIndex < row->GetLength())
			{
				characterLength = 1;
				character = row->GetAt(columnIndex);
				if (byteChecker.IsLeadByte(*(char*)*character))
				{
					characterLength = 2;
				}

				row->GetAt(columnIndex)->Select(true);
				columnIndex = row->Next();

				pagingBuffer->BeginSelectionIfNeeded();
				pagingBuffer->Next();
			}
			else
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}

			i += characterLength;
		}
	}
	else //3. 이동하지 않았으면,
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)option.findString);
		this->parent->MessageBox(message);
	}

	return ret;
}

bool Editor::FindNext() {
	//1. 검색결과 컨트롤러에서 검색방향에 따라 이동한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption option = searchResultController->GetFindReplaceOption();
	Long current = searchResultController->GetCurrent();
	Long previous = current;
	if (option.isSearchDown)
	{
		current = searchResultController->Next();
	}
	else
	{
		current = searchResultController->Previous();
	}

	//2. 선택해제한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 이동했으면,
	bool ret = false;
	if (current != previous)
	{
		ret = true;
		//2.1. 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(searchResultController->GetAt(current));
		caretNavigator.NormalizeColumn(0);

		//2.2. 선택한다.
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long characterLength;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < option.findString.GetLength())
		{
			characterLength = 0;
			if (columnIndex < row->GetLength())
			{
				characterLength = 1;
				character = row->GetAt(columnIndex);
				if (byteChecker.IsLeadByte(*(char*)*character))
				{
					characterLength = 2;
				}

				row->GetAt(columnIndex)->Select(true);
				columnIndex = row->Next();

				pagingBuffer->BeginSelectionIfNeeded();
				pagingBuffer->Next();
			}
			else
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}

			i += characterLength;
		}
	}
	else //3. 이동하지 않았으면,
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)option.findString);
		this->parent->MessageBox(message);
	}

	return ret;
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
