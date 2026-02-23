#include <afxwin.h>
#include "CtrlHAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlHAction::CtrlHAction(CWnd* parent)
	:Action(parent) {

}

CtrlHAction::~CtrlHAction() {

}

void CtrlHAction::Perform() {
#if 0
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_MENU_REPLACE, 0);
#endif
}