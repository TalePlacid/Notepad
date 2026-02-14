#include <afxwin.h>
#include "HScrollBarRightClickAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

HScrollBarRightClickAction::HScrollBarRightClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarRightClickAction::~HScrollBarRightClickAction() {

}

void HScrollBarRightClickAction::Perform() {
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