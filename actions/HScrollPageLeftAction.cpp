#include <afxwin.h>
#include "HScrollPageLeftAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

HScrollPageLeftAction::HScrollPageLeftAction(CWnd* parent)
	:Action(parent) {

}

HScrollPageLeftAction::~HScrollPageLeftAction() {

}

void HScrollPageLeftAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();
	Long pos = hScroll.GetPos() - hScroll.GetPage();
	if (pos < 0)
	{
		pos = 0;
	}
	scrollController->MoveHScroll(pos);
}

bool HScrollPageLeftAction::NeedScrollBarUpdate() {
	return false;
}

bool HScrollPageLeftAction::ShouldKeepSelection() {
	return true;
}
