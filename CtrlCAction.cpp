#include "CtrlCAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlCAction::CtrlCAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlCAction::~CtrlCAction() {

}

void CtrlCAction::Perform() {
	if (((NotepadForm*)(this->parent))->pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_COPY, 0);
	}
}