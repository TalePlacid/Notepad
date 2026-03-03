#include <afxwin.h>
#include "HScrollThumbTrackAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

HScrollThumbTrackAction::HScrollThumbTrackAction(CWnd* parent)
	:Action(parent) {

}

HScrollThumbTrackAction::~HScrollThumbTrackAction() {

}

void HScrollThumbTrackAction::Perform() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	scrollController->MoveHScroll(scrollInfo.nTrackPos);
}

bool HScrollThumbTrackAction::NeedScrollBarUpdate() {
	return false;
}

bool HScrollThumbTrackAction::ShouldKeepSelection() {
	return true;
}
