#include <afxwin.h>
#include "VScrollBarPageDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Caret.h"
#include "CaretController.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

VScrollBarPageDownAction::VScrollBarPageDownAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageDownAction::~VScrollBarPageDownAction() {

}

void VScrollBarPageDownAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	scrollInfo.nPos += scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.right - rect.left;

	Caret* caret = ((NotepadForm*)(this->parent))->caretController->GetCaret();
	Long y = caret->GetY() - scrollInfo.nPage;

	if (y < 0)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* originalRow = note->GetAt(rowIndex);
		Long columnIndex = originalRow->GetCurrent();

		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Glyph* character;
		Long originalWidth = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			character = originalRow->GetAt(i);
			originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		i = 0;
		while (i < note->GetLength() && y < 0)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
				rowIndex = note->GetCurrent();
			}
			y += sizeCalculator->GetRowHeight();
			i++;
		}

		Glyph* row = note->GetAt(rowIndex);
		Long previousWidth = 0;
		Long width = 0;
		i = 0;
		while (i < row->GetLength() && width < originalWidth)
		{
			character = row->GetAt(i);
			previousWidth = width;
			width += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		if (width - originalWidth > originalWidth - previousWidth)
		{
			i--;
		}

		row->Move(i);
		pagingBuffer->Move(i);
	}

	this->parent->Invalidate();
}