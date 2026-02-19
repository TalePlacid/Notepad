#include <afxwin.h>
#include "AutoWrapAction.h"
#include "../resource.h"
#include "../NotepadFrame.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../NoteWrapper.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

AutoWrapAction::AutoWrapAction(CWnd* parent)
	:Action(parent) {

}

AutoWrapAction::~AutoWrapAction() {

}

void AutoWrapAction::Perform() {
	PageLoader::LoadFirst(this->parent);

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->isAutoWrapped = !notepadForm->isAutoWrapped;
	NoteWrapper noteWrapper(this->parent);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
	Long max = fileRowCount * sizeCalculator->GetRowHeight();
	NotepadFrame* notepadFrame = (NotepadFrame*)(notepadForm->GetParent());
	if (notepadForm->isAutoWrapped)
	{
		Long dummyRowCount = noteWrapper.Wrap();
		max += dummyRowCount * sizeCalculator->GetRowHeight();
		notepadFrame->GetMenu()->CheckMenuItem(ID_MENU_AUTOWRAP, MF_CHECKED);
	}
	else
	{
		noteWrapper.Unwrap();
		notepadFrame->GetMenu()->CheckMenuItem(ID_MENU_AUTOWRAP, MF_UNCHECKED);
	}

	if (scrollController->HasVScroll())
	{
		scrollController->ResizeVRange(max);
	}

	pagingBuffer->UnmarkSelectionBegin();
	notepadForm->note->Select(false);
}