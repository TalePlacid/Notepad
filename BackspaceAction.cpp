#include <afxwin.h>
#include "resource.h"
#include "BackspaceAction.h"

#pragma warning(disable:4996)

BackspaceAction::BackspaceAction(CWnd* parent)
	:KeyAction(parent) {

}

BackspaceAction::~BackspaceAction() {
	
}

void BackspaceAction::Perform() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASE, 0);
}