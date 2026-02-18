#include <afxwin.h>
#include "CtrlShiftHomeAction.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../PageLoader.h"

#pragma warning(disable:4966)

CtrlShiftHomeAction::CtrlShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftHomeAction::~CtrlShiftHomeAction() {

}

void CtrlShiftHomeAction::Perform() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

	if (selectionBeginOffset < 0)
	{
		pagingBuffer->MarkSelectionBegin();
	}

	PageLoader::LoadFirst(this->parent);
}