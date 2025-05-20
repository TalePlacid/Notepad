#include <afxwin.h>
#include "VScrollBarDragAction.h"

#pragma warning(disable:4996)

VScrollBarDragAction::VScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	this->nPos = nPos;
}

VScrollBarDragAction::~VScrollBarDragAction() {

}

void VScrollBarDragAction::Perform() {
	SetScrollPos(this->parent->GetSafeHwnd(), SB_VERT, nPos, TRUE);
	
	this->parent->Invalidate();
}