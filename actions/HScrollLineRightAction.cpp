#include <afxwin.h>
#include "HScrollLineRightAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

HScrollLineRightAction::HScrollLineRightAction(CWnd* parent)
	:Action(parent) {

}

HScrollLineRightAction::~HScrollLineRightAction() {

}

void HScrollLineRightAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();

	Long averageWidth = ((NotepadForm*)(this->parent))->sizeCalculator->GetAverageCharacterWidth();
	Long pos = hScroll.GetPos() + averageWidth;
	Long posLimit = hScroll.GetMax() - hScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	scrollController->MoveHScroll(pos);
}

bool HScrollLineRightAction::NeedScrollBarUpdate() {
	return false;
}

bool HScrollLineRightAction::ShouldKeepSelection() {
	return true;
}
