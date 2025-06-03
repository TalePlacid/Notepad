#include <afxwin.h>
#include "CtrlShiftTAction.h"
#include "Glyph.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

CtrlShiftTAction::CtrlShiftTAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftTAction::~CtrlShiftTAction() {

}

void CtrlShiftTAction::Perform() {
	((NotepadForm*)(this->parent))->note->Select(FALSE);

	this->parent->Invalidate();
}
