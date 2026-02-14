#include <afxwin.h>
#include "CtrlShiftHomeAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"

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

	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);
}