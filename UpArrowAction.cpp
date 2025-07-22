#include <afxwin.h>
#include "UpArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

UpArrowAction::UpArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

UpArrowAction::~UpArrowAction() {

}

void UpArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* originalRow = note->GetAt(rowIndex);
	Long columnIndex = originalRow->GetCurrent();

	Glyph* character;
	Long originalWidth = 0;

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long i = 0;
	while (i < columnIndex)
	{
		character = originalRow->GetAt(i);
		originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	rowIndex = note->Previous();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->PreviousRow();
	if (!pagingBuffer->IsBelowTopLine())
	{
		pagingBuffer->Load();
		note = ((NotepadForm*)(this->parent))->note;
		rowIndex = note->GetCurrent();
	}

	Glyph* row = note->GetAt(rowIndex);
	Long previousWidth = 0;
	Long widthSum = 0;
	i = 0;
	while (i < row->GetLength() && widthSum < originalWidth)
	{
		character = row->GetAt(i);
		previousWidth = widthSum;
		widthSum += sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	if (widthSum - originalWidth > originalWidth - previousWidth)
	{
		i--;
	}

	pagingBuffer->Move(i);
	row->Move(i);

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();
	this->parent->Invalidate();
}