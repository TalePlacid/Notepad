#include <afxwin.h>
#include "CtrlMinusAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlMinusAction::CtrlMinusAction(CWnd* parent)
	: KeyAction(parent) {

}

CtrlMinusAction::~CtrlMinusAction() {

}

void CtrlMinusAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_MENU_ZOOMOUT, 0);
}