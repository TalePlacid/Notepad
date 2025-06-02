#include "CtrlCAction.h"
#include "NotepadForm.h"
#include "ClipboardController.h"

#pragma warning(disable:4996)

CtrlCAction::CtrlCAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlCAction::~CtrlCAction() {

}

void CtrlCAction::Perform() {
	((NotepadForm*)(this->parent))->clipboardController->Copy();
}