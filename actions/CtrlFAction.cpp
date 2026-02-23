#include <afxwin.h>
#include "CtrlFAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlFAction::CtrlFAction(CWnd* parent)
	:Action(parent) {

}

CtrlFAction::~CtrlFAction() {

}

void CtrlFAction::Perform() {
#if 0
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_MENU_FIND, 0);
#endif
}