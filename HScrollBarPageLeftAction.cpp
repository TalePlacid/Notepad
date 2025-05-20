#include <afxwin.h>
#include "HScrollBarPageLeftAction.h"

#pragma warning(disable:4996)

HScrollBarPageLeftAction::HScrollBarPageLeftAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarPageLeftAction::~HScrollBarPageLeftAction() {

}

void HScrollBarPageLeftAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS | SIF_PAGE;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	scrollInfo.nPos -= scrollInfo.nPage;
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	this->parent->Invalidate();
}