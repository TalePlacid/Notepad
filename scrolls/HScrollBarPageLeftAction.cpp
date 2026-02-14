#include <afxwin.h>
#include "HScrollBarPageLeftAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

HScrollBarPageLeftAction::HScrollBarPageLeftAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarPageLeftAction::~HScrollBarPageLeftAction() {

}

void HScrollBarPageLeftAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();
	Long pos = hScroll.GetPos() - hScroll.GetPage();
	if (pos < 0)
	{
		pos = 0;
	}
	scrollController->MoveHScroll(pos);
}