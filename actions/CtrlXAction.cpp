#include <afxwin.h>
#include "CtrlXAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

CtrlXAction::CtrlXAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlXAction::~CtrlXAction() {

}

void CtrlXAction::Perform() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		//SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_CUT, 0);
	}
}