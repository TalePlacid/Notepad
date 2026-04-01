#include "Editor.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
#include "SearchResultController.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "CaretNavigator.h"
#include "CoordinateConverter.h"
#include "NoteWrapper.h"
#include "PageManager.h"
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
	
	NoteWrapper noteWrapper(this->parent);
	Glyph* glyph;
	TCHAR character[2];
	Long dummied = 0;
	Long i = 0;
	while (i < text.GetLength())
	{
		//3.1. 문자를 읽는다.
		character[0] = text.GetAt(i);
		if (!ByteChecker::IsASCII(character) || character[0] == '\r')
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
		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
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

void Editor::EraseRange(Long frontOffset, Long rearOffset, Long& columnIndex) {
	//1. 앞 위치로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(frontOffset);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	columnIndex = currentColumnIndex;

	//3. 선택 길이만큼 반복한다.
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
			if (!ByteChecker::IsASCII(character))
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
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
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
		PageManager::ReloadAfterErase(this->parent);
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
			if (!ByteChecker::IsASCII(character))
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
			if (!ByteChecker::IsASCII(character))
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
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
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

	//8. 필요에 따라 선택한다.
	if (isSelected)
	{
		if (replacingText.GetLength() > 0)
		{
			pagingBuffer->MoveOffset(offset);
			pagingBuffer->MarkSelectionBegin();
			pagingBuffer->MoveOffset(offset + replacingText.GetLength());
		}
		else
		{
			pagingBuffer->UnmarkSelectionBegin();
		}
	}
}

void Editor::MoveUp() {
	//1. 이전 줄이 재적재범위에 들어가면, 재적재한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
	{
		PageManager::LoadPrevious(this->parent);
		rowIndex = note->GetCurrent();
	}

	//2. 이전 줄로 이동한다.
	Glyph* originalRow = note->GetAt(rowIndex);
	Long columnIndex = originalRow->GetCurrent();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long originalWidth = sizeCalculator->GetRowWidth(originalRow, columnIndex);

	Long previousRowIndex = note->Previous();
	Glyph* previousRow = note->GetAt(previousRowIndex);

	Long nearestIndex = sizeCalculator->GetNearestColumnIndex(previousRow, originalWidth);
	previousRow->Move(nearestIndex);

	//3. 페이징 버퍼에서 이동한다.
	Long characters;
	if (!originalRow->IsDummyRow())
	{
		pagingBuffer->PreviousRow();

		Long i = rowIndex - 1;
		while (i >= 0 && note->GetAt(i)->IsDummyRow())
		{
			i--;
		}

		characters = 0;
		while (i < rowIndex - 1)
		{
			characters += note->GetAt(i)->GetLength();
			i++;
		}
		characters += nearestIndex;

		pagingBuffer->Next(characters);
	}
	else
	{
		characters = columnIndex + previousRow->GetLength() - nearestIndex;
		pagingBuffer->Previous(characters);
	}
}

void Editor::MoveDown() {
	//1. 다음 줄이 재적재범위에 들어가면, 재적재한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	Long rowHeight = sizeCalculator->GetRowHeight();
	Scroll vScroll = scrollController->GetVScroll();
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Long lastPos = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * rowHeight;
	if (note->IsBelowBottomLine(rowIndex + 1) && lastPos < vScroll.GetMax())
	{
		PageManager::LoadNext(this->parent);
		rowIndex = note->GetCurrent();
	}

	//2. 마지막 줄이 아니면,
	if (rowIndex < note->GetLength() - 1)
	{
		//2.1. 다음 줄로 이동한다.
		Glyph* originalRow = note->GetAt(rowIndex);
		Long columnIndex = originalRow->GetCurrent();
		Long originalWidth = sizeCalculator->GetRowWidth(originalRow, columnIndex);

		rowIndex = note->Next();
		Glyph* nextRow = note->GetAt(rowIndex);
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(nextRow, originalWidth);
		nextRow->Move(nearestIndex);

		//2.2. 페이징 버퍼에서 이동한다.
		if (!nextRow->IsDummyRow())
		{
			pagingBuffer->NextRow();
			pagingBuffer->Move(nearestIndex);
		}
		else
		{
			Long characters = originalRow->GetLength() - columnIndex + nearestIndex;
			pagingBuffer->Next(characters);
		}
	}
}

void Editor::DragUp(CPoint point) {
	//1. 좌표를 노트 위치로 변환한다.
	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(point, rowIndex, columnIndex);
	
	//2. 현재 커서 정보를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	
	//3. 목표 줄까지 위로 선택 이동한다.
	Glyph* previousRow;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Scroll vScroll;
	Long difference;
	while (currentRowIndex > rowIndex)
	{
		//3.1. 현재 줄의 앞쪽을 선택한다.
		while (currentColumnIndex > 0)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentColumnIndex = row->Previous();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
		}
		
		//3.2. 필요하면 이전 페이지를 적재한다.
		if (note->IsAboveTopLine(currentRowIndex) && vScroll.GetPos() > 0)
		{
			difference = currentRowIndex - rowIndex;
			PageManager::LoadPrevious(this->parent);
			currentRowIndex = note->GetCurrent();
			rowIndex = currentRowIndex - difference;
		}
		
		//3.3. 이전 줄로 이동한다.
		previousRow = row;
		currentRowIndex = note->Previous();
		row = note->GetAt(currentRowIndex);
		currentColumnIndex = row->Last();
		if (!previousRow->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->PreviousRow();
			pagingBuffer->Last();
			pagingBuffer->EndSelectionIfCollapsed();
		}
		
		//3.4. 목표 칸까지 선택한다.
		while (currentColumnIndex > columnIndex)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentColumnIndex = row->Previous();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			pagingBuffer->Previous();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}
void Editor::DragDown(CPoint point) {
	//1. 좌표를 노트 위치로 변환한다.
	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(point, rowIndex, columnIndex);
	
	//2. 현재 커서 정보를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	
	//3. 목표 줄까지 아래로 선택 이동한다.
	Glyph* previousRow;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll;
	Long pageMax;
	Long difference;
	while (currentRowIndex < rowIndex)
	{
		//3.1. 현재 줄의 뒤쪽을 선택한다.
		while (currentColumnIndex < row->GetLength())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			currentColumnIndex = row->Next();
			pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
		}
		
		//3.2. 필요하면 다음 페이지를 적재한다.
		vScroll = scrollController->GetVScroll();
		pageMax = vScroll.GetPos() + vScroll.GetPage();
		if (note->IsBelowBottomLine(currentRowIndex + 1) && pageMax < vScroll.GetMax())
		{
			difference = rowIndex - currentRowIndex;
			PageManager::LoadNext(this->parent);
			currentRowIndex = note->GetCurrent();
			rowIndex = currentRowIndex + difference;
		}
		
		//3.3. 다음 줄로 이동한다.
		previousRow = row;
		currentRowIndex = note->Next();
		row = note->GetAt(currentRowIndex);
		currentColumnIndex = row->First();
		if (!row->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			pagingBuffer->NextRow();
			pagingBuffer->EndSelectionIfCollapsed();
		}
		
		//3.4. 목표 칸까지 선택한다.
		while (currentColumnIndex < columnIndex && currentColumnIndex < row->GetLength())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			row->GetAt(currentColumnIndex)->ToggleSelection();
			currentColumnIndex = row->Next();
			pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}
void Editor::DragLeft(CPoint point) {
	//1. 좌표를 노트 위치로 변환한다.
	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(point, rowIndex, columnIndex);
	
	//2. 현재 커서 정보를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	
	//3. 목표 칸까지 왼쪽으로 선택 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (currentColumnIndex > columnIndex)
	{
		pagingBuffer->BeginSelectionIfNeeded();
		currentColumnIndex = row->Previous();
		row->GetAt(currentColumnIndex)->ToggleSelection();
		pagingBuffer->Previous();
		pagingBuffer->EndSelectionIfCollapsed();
	}
}
void Editor::DragRight(CPoint point) {
	//1. 좌표를 노트 위치로 변환한다.
	CoordinateConverter coordinateConverter(this->parent);
	Long rowIndex;
	Long columnIndex;
	coordinateConverter.AbsoluteToNotePosition(point, rowIndex, columnIndex);
	
	//2. 현재 커서 정보를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long currentRowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(currentRowIndex);
	Long currentColumnIndex = row->GetCurrent();
	
	//3. 목표 칸까지 오른쪽으로 선택 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	while (currentColumnIndex < columnIndex)
	{
		pagingBuffer->BeginSelectionIfNeeded();
		row->GetAt(currentColumnIndex)->ToggleSelection();
		currentColumnIndex = row->Next();
		pagingBuffer->Next();
		pagingBuffer->EndSelectionIfCollapsed();
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

		//4.2. 위치를 찾았으면,
		if (nearestIndex >= 0)
		{
			//4.2.1. 선택을 해제한다.
			Glyph* note = ((NotepadForm*)(this->parent))->note;
			note->Select(false);
			pagingBuffer->UnmarkSelectionBegin();

			//4.2.2. 위치로 이동한다.
			CaretNavigator caretNavigator(this->parent);
			caretNavigator.MoveTo(searchResultController->GetAt(nearestIndex));
			caretNavigator.NormalizeColumn(0);

			//4.2.3. 선택한다.
			Long rowIndex = note->GetCurrent();
			Glyph* row = note->GetAt(rowIndex);
			Long columnIndex = row->GetCurrent();

			Glyph* character;
			Long characterLength;

			Long i = 0;
			while (i < option.findString.GetLength())
			{
				characterLength = 0;
				if (columnIndex < row->GetLength())
				{
					characterLength = 1;
					character = row->GetAt(columnIndex);
					if (!ByteChecker::IsASCII((char*)*character))
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
	//1. 현재 오프셋 기준으로 이전 검색결과를 다시 찾는다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption option = searchResultController->GetFindReplaceOption();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long targetOffset = currentOffset;
	Long current = -1;
	if (selectionBeginOffset >= 0 && selectionBeginOffset < currentOffset)
	{
		targetOffset = selectionBeginOffset;
	}
	if (option.isSearchDown)
	{
		current = searchResultController->FindNearestIndexAbove(targetOffset);
	}
	else
	{
		current = searchResultController->FindNearestIndexBelow(currentOffset);
	}

	//2. 선택해제한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 이동했으면,
	bool ret = false;
	if (current >= 0)
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
		
		Long i = 0;
		while (i < option.findString.GetLength())
		{
			characterLength = 0;
			if (columnIndex < row->GetLength())
			{
				characterLength = 1;
				character = row->GetAt(columnIndex);
				if (!ByteChecker::IsASCII((char*)*character))
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
	//1. 현재 오프셋 기준으로 다음 검색결과를 다시 찾는다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	FindReplaceOption option = searchResultController->GetFindReplaceOption();
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long targetOffset = currentOffset;
	if (selectionBeginOffset >= 0 && selectionBeginOffset < currentOffset)
	{
		targetOffset = selectionBeginOffset;
	}

	Long current = -1;
	if (option.isSearchDown)
	{
		current = searchResultController->FindNearestIndexBelow(currentOffset);
	}
	else
	{
		current = searchResultController->FindNearestIndexAbove(targetOffset);
	}

	//2. 선택해제한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 이동했으면,
	bool ret = false;
	if (current >= 0)
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
		
		Long i = 0;
		while (i < option.findString.GetLength())
		{
			characterLength = 0;
			if (columnIndex < row->GetLength())
			{
				characterLength = 1;
				character = row->GetAt(columnIndex);
				if (!ByteChecker::IsASCII((char*)*character))
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




