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

	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();

		Scroll vScroll = scrollController->GetVScroll();
		Long pos = vScroll.GetPos();
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long rowStartIndex = pagingBuffer->GetRowStartIndex();
		Long viewStartIndex = pos / rowHeight - rowStartIndex;

		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();

		this->viewRowIndex = rowIndex - viewStartIndex;

		NoteWrapper noteWrapper(this->parent);
		Long unwrapedDummyCount = noteWrapper.Unwrap();
		Long max = scrollController->GetVScroll().GetMax() - unwrapedDummyCount * rowHeight;
		scrollController->ResizeVRange(max);
	}
}

SuspendAutoWrap::~SuspendAutoWrap() {
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		NoteWrapper noteWrapper(this->parent);
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

		Long rowHeight = sizeCalculator->GetRowHeight();
		Long wrapedDummyCount = noteWrapper.Wrap();
		Long max = scrollController->GetVScroll().GetMax() + wrapedDummyCount * rowHeight;
		scrollController->ResizeVRange(max);
	
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Glyph* note = ((NotepadForm*)(this->parent))->note;
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