#include <afxwin.h>
#include "CtrlVAction.h"
#include "resource.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

CtrlVAction::CtrlVAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlVAction::~CtrlVAction() {

}

void CtrlVAction::Perform() {
	((NotepadForm*)(this->parent))->note->Select(false);
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_PASTE, NULL);
}