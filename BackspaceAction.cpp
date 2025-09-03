#include <afxwin.h>
#include "resource.h"
#include "BackspaceAction.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

BackspaceAction::BackspaceAction(CWnd* parent)
	:KeyAction(parent) {

}

BackspaceAction::~BackspaceAction() {
	
}

void BackspaceAction::Perform() {
	if (!((NotepadForm*)(this->parent))->IsCompositing())
	{
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (pagingBuffer->GetSelectionBeginOffset() < 0)
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASE, 0);
		}
		else
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASERANGE, 0);
		}
	}	
}