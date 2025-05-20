#include <afxwin.h>
#include "HScrollBarLeftClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

HScrollBarLeftClickAction::HScrollBarLeftClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarLeftClickAction::~HScrollBarLeftClickAction() {

}

void HScrollBarLeftClickAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	scrollInfo.nPos -= ((NotepadForm*)(this->parent))->sizeCalculator->GetMultiByteWidth();
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	this->parent->Invalidate();
}