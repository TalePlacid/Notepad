#include <afxwin.h>
#include "CtrlXAction.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlXAction::CtrlXAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlXAction::~CtrlXAction() {

}

void CtrlXAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_CUT, 0);
}