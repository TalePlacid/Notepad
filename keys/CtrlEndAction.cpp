#include <afxwin.h>
#include "CtrlEndAction.h"
#include "resource.h"

#pragma warning(disable:4996)

CtrlEndAction::CtrlEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlEndAction::~CtrlEndAction() {

}

void CtrlEndAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADLAST, 0);
}