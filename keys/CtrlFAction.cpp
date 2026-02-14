#include <afxwin.h>
#include "CtrlFAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlFAction::CtrlFAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlFAction::~CtrlFAction() {

}

void CtrlFAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_MENU_FIND, 0);
}