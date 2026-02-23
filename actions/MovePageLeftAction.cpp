#include <afxwin.h>
#include "MovePageLeftAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

MovePageLeftAction::MovePageLeftAction(CWnd* parent)
	:Action(parent) {

}

MovePageLeftAction::~MovePageLeftAction() {

}

void MovePageLeftAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (scrollController->HasHScroll())
	{
		Scroll hScroll = scrollController->GetHScroll();
		Long pos = hScroll.GetPos() - hScroll.GetPage();
		if (pos < 0)
		{
			pos = 0;
		}
		scrollController->MoveHScroll(pos);
	}
}