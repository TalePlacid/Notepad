#include <afxwin.h>
#include "SelectPageDownAction.h"
#include "../ScrollController.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../PagingBuffer.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

SelectPageDownAction::SelectPageDownAction(CWnd* parent)
	:Action(parent) {

}

SelectPageDownAction::~SelectPageDownAction() {

}

void SelectPageDownAction::Perform() {
	//1. 수직 스크롤바가 존재하고, 마지막 위치가 이니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Long posLimit = vScroll.GetMax() - vScroll.GetPage();
	if (scrollController->HasVScroll() && vScroll.GetPos() < posLimit)
	{
		//1.1. 내려갈 줄 수를 구한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long rowCount = vScroll.GetPage() / rowHeight;
		if (vScroll.GetPage() % rowHeight > 0)
		{
			rowCount++;
		}

		//1.2. 노트에서 현재 줄의 너비를 구한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Long originalRowWidth = sizeCalculator->GetRowWidth(rowIndex, columnIndex);

		//1.3. 마지막줄이 아니고, 내려가야할 줄 수가 남았으면,
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long bytes;
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		Long i = 0;
		while (rowIndex + 1 < note->GetLength() && i < rowCount)
		{
			//1.3.1. 적재범위를 넘어서면, 재적재한다.
			if (!note->IsLastPage() && note->IsBelowBottomLine(rowIndex + 1))
			{
				PageManager::LoadNext(this->parent);
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
			}

			//3.2. 줄의 끝까지 반복한다.
			while (columnIndex < row->GetLength())
			{
				row->GetAt(columnIndex)->ToggleSelection();
				bytes = row->GetAt(columnIndex)->GetBytes();
				columnIndex = row->Next();

				pagingBuffer->BeginSelectionIfNeeded();
				currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
				pagingBuffer->EndSelectionIfCollapsed();
			}

			//3.3. 노트에서 이동한다.
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			if (!row->IsDummyRow())
			{
				pagingBuffer->BeginSelectionIfNeeded();
				currentOffset = pagingBuffer->MoveOffset(currentOffset + 2);
				pagingBuffer->EndSelectionIfCollapsed();
			}

			//3.4. 가장 비슷한 위치까지 반복한다.
			Long nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, originalRowWidth);
			while (columnIndex < nearestIndex)
			{
				row->GetAt(columnIndex)->ToggleSelection();
				bytes = row->GetAt(columnIndex)->GetBytes();
				columnIndex = row->Next();

				pagingBuffer->BeginSelectionIfNeeded();
				currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
				pagingBuffer->EndSelectionIfCollapsed();
			}

			i++;
		}
	}
}
