#include <afxwin.h>
#include "RightArrowAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

RightArrowAction::RightArrowAction(CWnd* parent)
	:KeyAction(parent) {

}

RightArrowAction::~RightArrowAction() {

}

void RightArrowAction::Perform() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (row->GetCurrent() < row->GetLength())
	{
		row->Next();
		pagingBuffer->Next();
	}
	else
	{
		if (note->GetCurrent() < note->GetLength() - 1)
		{
			rowIndex = note->Next();
			pagingBuffer->NextRow();
			if (!pagingBuffer->IsAboveBottomLine())
			{
				pagingBuffer->Load();
				note = ((NotepadForm*)(this->parent))->note;
				rowIndex = note->GetCurrent();
			}

			row = note->GetAt(rowIndex);
			row->First();
			pagingBuffer->First();
		}
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();
	this->parent->Invalidate();
}