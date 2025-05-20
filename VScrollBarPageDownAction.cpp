#include <afxwin.h>
#include "VScrollBarPageDownAction.h"

#pragma warning(disable:4996)

VScrollBarPageDownAction::VScrollBarPageDownAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarPageDownAction::~VScrollBarPageDownAction() {

}

void VScrollBarPageDownAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS | SIF_PAGE;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	scrollInfo.nPos += scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	this->parent->Invalidate();\
}