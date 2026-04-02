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
	this->currentRowScreenDelta = 0;
	this->currentColumnIndex = 0;
	this->isSuspended = FALSE;

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	if (notepadForm->IsAutoWrapped())
	{
		if (notepadForm->GetAutoWrapSuspendCount() == 0)
		{
			//1. 화면상 몇 번째 줄에 있었는지를 구한다.
			ScrollController* scrollController = notepadForm->scrollController;
			SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;
			Long rowHeight = sizeCalculator->GetRowHeight();

			Scroll vScroll = scrollController->GetVScroll();
			Long pos = vScroll.GetPos();
			PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
			Long rowStartIndex = pagingBuffer->GetRowStartIndex();
			Long viewStartIndex = pos / rowHeight - rowStartIndex;

			Glyph* note = notepadForm->note;
			Glyph* currentRow = note->GetAt(note->GetCurrent());
			this->currentRowScreenDelta = note->GetCurrent() - viewStartIndex;
			this->currentColumnIndex = currentRow->GetCurrent();

			//2. 수직 스크롤 위치를 갱신한다.
			Long dummyRowCount = 0;
			Long i = 0;
			while (i <= note->GetCurrent() && i < note->GetLength())
			{
				if (note->GetAt(i)->IsDummyRow())
				{
					dummyRowCount++;
				}
				i++;
			}
			Long unwrappedPos = scrollController->GetVScroll().GetPos() - dummyRowCount * rowHeight;
			scrollController->MoveVScroll(unwrappedPos);

			//3. 노트의 자동개행을 푼다.
			NoteWrapper noteWrapper(this->parent);
			Long unwrappedDummyCount = noteWrapper.Unwrap();

			//4. 스크롤 최대값을 갱신한다.
			Long max = scrollController->GetVScroll().GetMax() - unwrappedDummyCount * rowHeight;
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
			//1. 자동개행한다.
			NoteWrapper noteWrapper(this->parent);
			Long wrappedDummyCount = noteWrapper.Wrap();

			//2. 스크롤 최대값을 갱신한다.
			ScrollController* scrollController = notepadForm->scrollController;
			SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;
			Long rowHeight = sizeCalculator->GetRowHeight();
			Long max = scrollController->GetVScroll().GetMax() + wrappedDummyCount * rowHeight;
			scrollController->ResizeVRange(max);

			//3. 현재줄을 기존의 화면상 줄 위치로 스크롤 이동시킨다.
			PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
			Glyph* note = notepadForm->note;
			Long rowIndex = note->GetCurrent();
			Glyph* row = note->GetAt(rowIndex);
			Long wrappedColumnIndex = row->GetCurrent();
			Long lastColumnIndex = row->GetLength();

			if (this->currentColumnIndex == 0 && wrappedColumnIndex > 0)
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				row->First();
			}
			else if (this->currentColumnIndex > 0 && wrappedColumnIndex == 0)
			{
				rowIndex = note->Previous();
				row = note->GetAt(rowIndex);
				row->Last();
			}

			Long rowStartIndex = pagingBuffer->GetRowStartIndex();
			Long pos = (rowStartIndex + rowIndex - this->currentRowScreenDelta) * rowHeight;
			
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
