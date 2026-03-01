#include <afxwin.h>
#include "SelectDocFrontAction.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../PageLoader.h"

#pragma warning(disable:4966)

SelectDocFrontAction::SelectDocFrontAction(CWnd* parent)
	:Action(parent) {

}

SelectDocFrontAction::~SelectDocFrontAction() {

}

void SelectDocFrontAction::Perform() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

	if (selectionBeginOffset < 0)
	{
		pagingBuffer->MarkSelectionBegin();
	}

	PageLoader::LoadFirst(this->parent);
}
