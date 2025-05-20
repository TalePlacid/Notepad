#include <afxwin.h>
#include "HScrollBarRightClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

HScrollBarRightClickAction::HScrollBarRightClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarRightClickAction::~HScrollBarRightClickAction() {

}

void HScrollBarRightClickAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	scrollInfo.nPos += ((NotepadForm*)(this->parent))->sizeCalculator->GetMultiByteWidth();
	SetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo, TRUE);

	this->parent->Invalidate();
}