#include <afxwin.h>
#include "MovePageRightAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

MovePageRightAction::MovePageRightAction(CWnd* parent)
	:Action(parent) {

}

MovePageRightAction::~MovePageRightAction() {

}

void MovePageRightAction::Perform() {
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