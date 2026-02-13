#include "UpdateScrollBarsCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

UpdateScrollBarsCommand::UpdateScrollBarsCommand(CWnd* parent, ScrollBarNeeded scrollBarNeeded)
	:Command(parent) {
	this->scrollBarNeeded = scrollBarNeeded;
}

UpdateScrollBarsCommand::~UpdateScrollBarsCommand() {

}

void UpdateScrollBarsCommand::Execute() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	RECT clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
	Long clientAreaWidth = clientArea.right - clientArea.left;
	Long clientAreaHeight = clientArea.bottom - clientArea.top;

	Glyph* note = notepadForm->note;
	SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;

	Long max = note->GetLength() * sizeCalculator->GetRowHeight();
	Long page = clientAreaHeight;
	Long pos = 0;

	int a = this->scrollBarNeeded & SHOW_VSCROLL;
	if (!notepadForm->hasVScrollBar && (this->scrollBarNeeded & SHOW_VSCROLL))
	{
		this->parent->ModifyStyle(0, WS_VSCROLL);
		notepadForm->hasVScrollBar = TRUE;
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long rowCount = pagingBuffer->CountRow(pagingBuffer->GetEndOffset());
		max = rowCount * sizeCalculator->GetRowHeight();
	}

	ScrollController* scrollController = notepadForm->scrollController;
	scrollController->ResizeVRange(max);
	scrollController->ResizeVPage(page);

	SCROLLINFO scrollInfo = { 0, };
	Scroll scroll = scrollController->GetVScroll();

	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin = scroll.GetMin();
	scrollInfo.nMax = scroll.GetMax();
	scrollInfo.nPage = scroll.GetPage();
	scrollInfo.nPos = scroll.GetPos();

	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);
	if (!(this->scrollBarNeeded & SHOW_VSCROLL))
	{
		this->parent->ModifyStyle(WS_VSCROLL, 0);
	}

	this->parent->SetWindowPos(
		NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
	);
}