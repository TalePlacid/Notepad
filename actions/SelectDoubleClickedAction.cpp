#include <afxwin.h>
#include "SelectDoubleClickedAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../CoordinateConverter.h"

#pragma warning(disable:4996)

SelectDoubleClickedAction::SelectDoubleClickedAction(CWnd* parent, CPoint point)
	:Action(parent), point(point) {

}

SelectDoubleClickedAction::~SelectDoubleClickedAction() {

}

void SelectDoubleClickedAction::Perform() {
	//1. 더블 클릭한 위치를 알아낸다.
	CoordinateConverter coordinateConverter(this->parent);
	Long clickedRowIndex;
	Long clickedColumnIndex;
	coordinateConverter.AbsoluteToNotePosition(this->point, clickedRowIndex, clickedColumnIndex);

	//2. 해당 위치가 단어 시작점이 아니라면, 해당 위치 이전의 단어 시작점을 찾는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Glyph* clickedRow = note->GetAt(clickedRowIndex);
	Long wordStart = clickedColumnIndex;
	if (clickedColumnIndex > 0 && clickedRow->GetAt(clickedColumnIndex - 1)->IsWordCharacter())
	{
		wordStart = clickedRow->FindPreviousWordStart(clickedColumnIndex);
	}

	//3. 단어 시작점으로 이동한다.
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();

	//첫 칸으로 이동
	Long bytes = row->GetPreviousBytes(columnIndex);
	columnIndex = row->First();
	currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
	
	//위로 이동.
	while (rowIndex > clickedRowIndex)
	{
		bytes = 0;
		if (!row->IsDummyRow())
		{
			bytes += 2;
		}
		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		bytes += row->GetBytes();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
	}

	//아래로 이동
	while (rowIndex < clickedRowIndex)
	{
		bytes = row->GetBytes();
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		if (!row->IsDummyRow())
		{
			bytes += 2;
		}
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}

	//단어 시작까지 이동
	while (columnIndex < wordStart)
	{
		bytes = row->GetAt(columnIndex)->GetBytes();
		columnIndex = row->Next();
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}

	//4. 기존 선택을 취소한다.
	note->Select(FALSE);
	pagingBuffer->UnmarkSelectionBegin();

	//5. 단어 끝까지 선택한다.
	Glyph* character;
	Long i = wordStart;
	while (i < row->GetLength() && row->GetAt(i)->IsWordCharacter())
	{
		pagingBuffer->BeginSelectionIfNeeded();
		character = row->GetAt(i);
		bytes = character->GetBytes();
		character->Select(true);
		columnIndex = row->Next();
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
		i++;
	}
}