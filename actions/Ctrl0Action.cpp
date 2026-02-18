#include <afxwin.h>
#include "Ctrl0Action.h"
#include "../resource.h"

#pragma warning(disable:4996)

Ctrl0Action::Ctrl0Action(CWnd* parent)
	:KeyAction(parent) {

}

Ctrl0Action::~Ctrl0Action() {

}

void Ctrl0Action::Perform() {
#if 0
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_MENU_ZOOMRESET, 0);
#endif
}