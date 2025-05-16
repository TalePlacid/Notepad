#include <afxwin.h>
#include "CtrlTAction.h"
#include "NotepadForm.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

CtrlTAction::CtrlTAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlTAction::~CtrlTAction() {

}

void CtrlTAction::Perform() {
	NoteWrapper noteWrapper(this->parent);

	noteWrapper.InsertDummyRows();

	this->parent->Invalidate();
}