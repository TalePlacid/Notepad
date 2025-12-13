#include <afxwin.h>
#include "CtrlPageUpAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

CtrlPageUpAction::CtrlPageUpAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlPageUpAction::~CtrlPageUpAction() {

}

void CtrlPageUpAction::Perform() {
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