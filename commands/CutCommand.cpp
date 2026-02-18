#include <afxwin.h>
#include "CutCommand.h"
#include "../resource.h"

CutCommand::CutCommand(CWnd* parent)
	:Command(parent) {

}

CutCommand::~CutCommand() {

}

void CutCommand::Execute() {
#if 0
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_COPY, 0);
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASERANGE, 0);
#endif
}