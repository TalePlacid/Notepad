#include <afxwin.h>
#include "MovePageUpAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

MovePageUpAction::MovePageUpAction(CWnd* parent)
	:Action(parent) {

}

MovePageUpAction::~MovePageUpAction() {

}

void MovePageUpAction::Perform() {
	//1. 수직스크롤바가 있고, 처음 위치가 아니라면,
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	if (scrollController->HasVScroll() && vScroll.GetPos() > 0)
	{
		//1.1. 올라갈 줄 수를 구한다.
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

		//1.3. 줄의 처음으로 이동한다.
		Long movedIndex = row->First();
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		Long bytes = row->GetPreviousBytes(columnIndex);
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

		//1.3. 줄 수 만큼, 첫번째 줄이 아니면 반복한다.
		Glyph* previousRow;
		Long previousColumnIndex;
		Long rowBytes;
		Long previousRowBytes;
		Long i = 0;
		while (i < rowCount && rowIndex > 0)
		{
			//1.3.1. 적재범위를 벗어나면, 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageManager::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
			}

			//1.3.2. 노트에서 이동한다.
			previousRow = note->GetAt(rowIndex);
			previousColumnIndex = previousRow->GetCurrent();
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			//1.3.3. 페이징 버퍼에서 이동한다.
			previousRowBytes = previousRow->GetPreviousBytes(previousColumnIndex);
			rowBytes = row->GetBytes();
			bytes = previousRowBytes + rowBytes;
			if (!previousRow->IsDummyRow())
			{
				bytes += 2;
			}
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

			i++;
		}

		//1.4. 줄 너비와 가장 가까운 위치로 이동한다.
		Long nearestIndex = sizeCalculator->GetNearestColumnIndex(rowIndex, originalRowWidth);
		movedIndex = row->Move(nearestIndex);
		bytes = row->GetPreviousBytes(movedIndex);
		currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
	}
}
