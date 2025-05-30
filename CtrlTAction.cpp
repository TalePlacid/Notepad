#include <afxwin.h>
#include "CtrlTAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlTAction::CtrlTAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlTAction::~CtrlTAction() {

}

void CtrlTAction::Perform() {
	((NotepadForm*)(this->parent))->note->Select(FALSE);

	this->parent->Invalidate();
}