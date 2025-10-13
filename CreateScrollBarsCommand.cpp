#include <afxwin.h>
#include "CreateScrollBarsCommand.h"
#include "NotepadForm.h"
#include "ScrollBarAdapter.h"
#include "ScrollBarNecessityChecker.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

CreateScrollBarsCommand::CreateScrollBarsCommand(CWnd* parent)
	:Command(parent) {

}

CreateScrollBarsCommand::~CreateScrollBarsCommand() {

}

void CreateScrollBarsCommand::Execute() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long contentsHeight = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset()) * sizeCalculator->GetRowHeight();

	RECT clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);

	ScrollBarNecessityChecker scrollBarNecessityChecker;
	bool vScrollNeeded;
	bool hScrollNeeded;
	Long scrollBarThickness = GetSystemMetrics(SM_CXVSCROLL);
	scrollBarNecessityChecker.Check(0, contentsHeight, clientArea, scrollBarThickness, vScrollNeeded, hScrollNeeded);

	if (vScrollNeeded)
	{
		ScrollBarAdapter* scrollBarAdapter = ((NotepadForm*)(this->parent))->scrollBarAdapter;
		scrollBarAdapter->Create(SB_VERT, 0, contentsHeight, clientArea.bottom - clientArea.top, 0);
	}
}