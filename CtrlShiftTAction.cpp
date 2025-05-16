#include <afxwin.h>
#include "CtrlShiftTAction.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

CtrlShiftTAction::CtrlShiftTAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlShiftTAction::~CtrlShiftTAction() {

}

void CtrlShiftTAction::Perform() {
	NoteWrapper noteWrapper(this->parent);

	noteWrapper.DeleteDummyRows();

	this->parent->Invalidate();
}
