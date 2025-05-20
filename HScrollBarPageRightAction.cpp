#include <afxwin.h>
#include "HScrollBarPageRightAction.h"

#pragma warning(disable:4996)

HScrollBarPageRightAction::HScrollBarPageRightAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarPageRightAction::~HScrollBarPageRightAction() {

}

void HScrollBarPageRightAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS | SIF_PAGE;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	scrollInfo.nPos += scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	this->parent->Invalidate();
}