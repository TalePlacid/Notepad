#include <afxwin.h>
#include "UpArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

UpArrowAction::UpArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

UpArrowAction::~UpArrowAction() {

}

void UpArrowAction::Perform() {
	//1. 이전 줄이 재적재범위에 들어가면, 재적재한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, ID_COMMAND_LOADPREVIOUS, 0);
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

	//3.  페이징 버퍼에서 이동한다.
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