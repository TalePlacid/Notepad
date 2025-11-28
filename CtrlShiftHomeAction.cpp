#include <afxwin.h>
#include "CtrlShiftHomeAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "resource.h"

#pragma warning(disable:4966)

CtrlShiftHomeAction::CtrlShiftHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftHomeAction::~CtrlShiftHomeAction() {

}

void CtrlShiftHomeAction::Perform() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

	MarkingHelper markingHelper(this->parent);
	if (selectionBeginOffset < 0)
	{
		markingHelper.Mark();
	}

	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);
}