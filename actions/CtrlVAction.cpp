#include <afxwin.h>
#include "CtrlVAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

CtrlVAction::CtrlVAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlVAction::~CtrlVAction() {

}

void CtrlVAction::Perform() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		//SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASERANGE, 0);
	}

	((NotepadForm*)(this->parent))->note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();
	
	//SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_PASTE, 0);
}