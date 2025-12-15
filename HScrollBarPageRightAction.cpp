#include <afxwin.h>
#include "HScrollBarPageRightAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

HScrollBarPageRightAction::HScrollBarPageRightAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarPageRightAction::~HScrollBarPageRightAction() {

}

void HScrollBarPageRightAction::Perform() {
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