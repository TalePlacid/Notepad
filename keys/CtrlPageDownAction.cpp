#include <afxwin.h>
#include "CtrlPageDownAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

CtrlPageDownAction::CtrlPageDownAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlPageDownAction::~CtrlPageDownAction() {

}

void CtrlPageDownAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (scrollController->HasHScroll())
	{
		Scroll hScroll = scrollController->GetHScroll();
		Long pos = hScroll.GetPos() + hScroll.GetPage();
		Long posLimit = hScroll.GetMax() - hScroll.GetPage();
		if (pos > posLimit)
		{
			pos = posLimit;
		}

		scrollController->MoveHScroll(pos);
	}
}