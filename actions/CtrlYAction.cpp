#include <afxwin.h>
#include "CtrlYAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

CtrlYAction::CtrlYAction(CWnd* parent)
	:Action(parent) {

}

CtrlYAction::~CtrlYAction() {

}

void CtrlYAction::Perform() {
	//SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_REDO, 0);
}