#include <afxwin.h>
#include "HScrollLineLeftAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

HScrollLineLeftAction::HScrollLineLeftAction(CWnd* parent)
	:Action(parent) {

}

HScrollLineLeftAction::~HScrollLineLeftAction() {

}

void HScrollLineLeftAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();

	Long averageWidth = ((NotepadForm*)(this->parent))->sizeCalculator->GetAverageCharacterWidth();
	Long pos = hScroll.GetPos() - averageWidth;
	if (pos < 0)
	{
		pos = 0;
	}
	scrollController->MoveHScroll(pos);
}

bool HScrollLineLeftAction::NeedScrollBarUpdate() {
	return false;
}

bool HScrollLineLeftAction::ShouldKeepSelection() {
	return true;
}
