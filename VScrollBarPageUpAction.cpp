#include <afxwin.h>
#include "VScrollBarPageUpAction.h"

#pragma warning(disable:4996)

VScrollBarPageUpAction::VScrollBarPageUpAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageUpAction::~VScrollBarPageUpAction() {

}

void VScrollBarPageUpAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS | SIF_PAGE;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	scrollInfo.nPos -= scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	this->parent->Invalidate();
}