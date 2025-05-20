#include <afxwin.h>
#include "VScrollBarUpClickAction.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

VScrollBarUpClickAction::VScrollBarUpClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

VScrollBarUpClickAction::~VScrollBarUpClickAction() {

}

void VScrollBarUpClickAction::Perform() {
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);
	
	scrollInfo.nPos -= ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();

	SetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo, TRUE);

	this->parent->Invalidate();
}