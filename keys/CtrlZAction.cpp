#include <afxwin.h>
#include "CtrlZAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlZAction::CtrlZAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlZAction::~CtrlZAction() {

}

void CtrlZAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_UNDO, 0);
}

