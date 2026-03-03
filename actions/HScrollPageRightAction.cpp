#include <afxwin.h>
#include "HScrollPageRightAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

HScrollPageRightAction::HScrollPageRightAction(CWnd* parent)
	:Action(parent) {

}

HScrollPageRightAction::~HScrollPageRightAction() {

}

void HScrollPageRightAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();
	Long pos = hScroll.GetPos() + hScroll.GetPage();
	Long posLimit = hScroll.GetMax() - hScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	scrollController->MoveHScroll(pos);
}

bool HScrollPageRightAction::NeedScrollBarUpdate() {
	return false;
}

bool HScrollPageRightAction::ShouldKeepSelection() {
	return true;
}
