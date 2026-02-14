#include <afxwin.h>
#include "CtrlPlusAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlPlusAction::CtrlPlusAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlPlusAction::~CtrlPlusAction() {

}

void CtrlPlusAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_MENU_ZOOMIN, 0);
}