#include <afxwin.h>
#include "HScrollBarDragAction.h"

#pragma warning(disable:4996)

HScrollBarDragAction::HScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	this->nPos = nPos;
}

HScrollBarDragAction::~HScrollBarDragAction() {

}

void HScrollBarDragAction::Perform() {
	SetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ, this->nPos, TRUE);

	this->parent->Invalidate();
}