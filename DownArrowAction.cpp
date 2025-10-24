#include <afxwin.h>
#include "DownArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "ScrollController.h"
#include "resource.h"

#pragma warning(disable:4996)

DownArrowAction::DownArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

DownArrowAction::~DownArrowAction() {

}

void DownArrowAction::Perform() {
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

	rowIndex = note->Next();
	Glyph* nextRow = note->GetAt(rowIndex);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->NextRow();

	Long previousWidth = 0;
	Long width = 0;
	i = 0;
	while (width < originalWidth && i < nextRow->GetLength())
	{
		character = nextRow->GetAt(i);
		previousWidth = width;
		width += sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	if (width - originalWidth > originalWidth - previousWidth)
	{
		i--;
	}

	pagingBuffer->Move(i);
	nextRow->Move(i);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();

	Long currentHeight = (rowIndex + 1) * sizeCalculator->GetRowHeight();
	if (currentHeight > vScroll.GetPage())
	{
		scrollController->Down();
	}

	if (!note->IsAboveBottomLine())
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, ID_COMMAND_LOAD, 0);
	}

	note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

	this->parent->Invalidate();
}