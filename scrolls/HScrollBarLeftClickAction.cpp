#include <afxwin.h>
#include "HScrollBarLeftClickAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

HScrollBarLeftClickAction::HScrollBarLeftClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarLeftClickAction::~HScrollBarLeftClickAction() {

}

void HScrollBarLeftClickAction::Perform() {
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