#include <afxwin.h>
#include "DownArrowAction_Fixed.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

DownArrowAction_Fixed::DownArrowAction_Fixed(CWnd* parent)
	:KeyAction(parent) {

}

DownArrowAction_Fixed::~DownArrowAction_Fixed() {

}

void DownArrowAction_Fixed::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* originalRow = note->GetAt(rowIndex);
	Long columnIndex = originalRow->GetCurrent();

	Glyph* character;
	TCHAR(*content);
	Long originalWidth = 0;

	Long i = 0;
	while (i < columnIndex)
	{
		character = originalRow->GetAt(i);
		originalWidth += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	rowIndex = note->Next();
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->NextRow();
	if (!pagingBuffer->IsAboveBottomLine())
	{
		pagingBuffer->Load();
		note = ((NotepadForm*)(this->parent))->note;
		rowIndex = note->GetCurrent();
	}
	Glyph* nextRow = note->GetAt(rowIndex);
	
	Long previousWidth = 0;
	Long width = 0;
	i = 0;
	while (width < originalWidth && i < nextRow->GetLength())
	{
		character = nextRow->GetAt(i);
		previousWidth = width;
		width += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	if (width - originalWidth > originalWidth - previousWidth)
	{
		i--;
	}

	pagingBuffer->Move(i);
	nextRow->Move(i);

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->note->Select(false);
	this->parent->Invalidate();
}