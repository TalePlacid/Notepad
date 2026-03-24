#include <afxwin.h>
#include "SuspendAutoWrap.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

SuspendAutoWrap::SuspendAutoWrap(CWnd* parent) {
	this->parent = parent;
	this->viewRowIndex = 0;
	this->isSuspended = FALSE;

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	if (notepadForm->IsAutoWrapped())
	{
		if (notepadForm->GetAutoWrapSuspendCount() == 0)
		{
			ScrollController* scrollController = notepadForm->scrollController;
			SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;
			Long rowHeight = sizeCalculator->GetRowHeight();

			Scroll vScroll = scrollController->GetVScroll();
			Long pos = vScroll.GetPos();
			PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
			Long rowStartIndex = pagingBuffer->GetRowStartIndex();
			Long viewStartIndex = pos / rowHeight - rowStartIndex;

			Glyph* note = notepadForm->note;
			Long rowIndex = note->GetCurrent();

			this->viewRowIndex = rowIndex - viewStartIndex;

			NoteWrapper noteWrapper(this->parent);
			Long unwrapedDummyCount = noteWrapper.Unwrap();
			Long max = scrollController->GetVScroll().GetMax() - unwrapedDummyCount * rowHeight;
			scrollController->ResizeVRange(max);
			this->isSuspended = TRUE;
		}

		notepadForm->IncreaseAutoWrapSuspendCount();
	}
}

SuspendAutoWrap::~SuspendAutoWrap() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	if (notepadForm->IsAutoWrapped())
	{
		notepadForm->DecreaseAutoWrapSuspendCount();

		if (this->isSuspended && notepadForm->GetAutoWrapSuspendCount() == 0)
		{
			NoteWrapper noteWrapper(this->parent);
			ScrollController* scrollController = notepadForm->scrollController;
			SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;

			Long rowHeight = sizeCalculator->GetRowHeight();
			Long wrapedDummyCount = noteWrapper.Wrap();
			Long max = scrollController->GetVScroll().GetMax() + wrapedDummyCount * rowHeight;
			scrollController->ResizeVRange(max);
			
			PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
			Glyph* note = notepadForm->note;
			Long rowIndex = note->GetCurrent();
			Long rowStartIndex = pagingBuffer->GetRowStartIndex();
			Long pos = (rowStartIndex + rowIndex - this->viewRowIndex) * rowHeight;
			
			Scroll vScroll = scrollController->GetVScroll();
			Long posLimit = vScroll.GetMax() - vScroll.GetPage();
			if (pos > posLimit)
			{
				pos = posLimit;
			}
			scrollController->MoveVScroll(pos);
		}
	}
}
