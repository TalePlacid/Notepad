#include <afxwin.h>
#include "SelectWordRightAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

SelectWordRightAction::SelectWordRightAction(CWnd* parent)
	:Action(parent) {

}

SelectWordRightAction::~SelectWordRightAction() {

}

void SelectWordRightAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 줄의 끝이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset;
	if (columnIndex < row->GetLength())
	{
		//2.1. 노트에서 다음 단어 시작으로 이동한다.
		Long wordStart = row->FindNextWordStart(columnIndex);
		Long movedIndex = row->Move(wordStart);

		//2.2. 노트에서 범위만큼 선택한다.
		row->ToggleSelection(columnIndex, movedIndex);

		//2.3. 차이만큼 반복한다.
		Long difference = movedIndex - columnIndex;
		Long i = 0;
		while (i < difference)
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentOffset = pagingBuffer->Next();
			pagingBuffer->EndSelectionIfCollapsed();
			i++;
		}
	}
	else //3. 줄의 끝이면,
	{
		//3.1. 적재범위를 벗어나면, 재적재한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * sizeCalculator->GetRowHeight();
		if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
		{
			PageLoader::LoadNext(this->parent);
		}

		//3.2. 노트에서 이동한다.
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		//3.3. 줄이 진짜이면,
		if (!row->IsDummyRow())
		{
			pagingBuffer->BeginSelectionIfNeeded();
			currentOffset = pagingBuffer->NextRow();
			pagingBuffer->EndSelectionIfCollapsed();
		}
	}
}
