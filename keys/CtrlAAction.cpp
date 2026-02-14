#include <afxwin.h>
#include "CtrlAAction.h"
#include "../resource.h"
#include "../PagingBuffer.h"
#include "../NotepadForm.h"

#pragma warning(disable:4996)

CtrlAAction::CtrlAAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlAAction::~CtrlAAction() {

}

void CtrlAAction::Perform() {
	//1. 페이징 버퍼에서 첫 위치로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->FirstRow();
	pagingBuffer->MarkSelectionBegin();

	//2. 마지막 페이지를 적재한다.
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADLAST, 0);
}