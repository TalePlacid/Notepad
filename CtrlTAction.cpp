#include <afxwin.h>
#include "CtrlTAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ClipboardController.h"

#pragma warning(disable:4996)

CtrlTAction::CtrlTAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlTAction::~CtrlTAction() {

}

void CtrlTAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	notepadForm->clipboardController->Copy();

	this->parent->Invalidate();
}