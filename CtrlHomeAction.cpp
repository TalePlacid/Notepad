#include <afxwin.h>
#include "CtrlHomeAction.h"
#include "MarkingHelper.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlHomeAction::CtrlHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlHomeAction::~CtrlHomeAction() {

}

void CtrlHomeAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);
}