#include "MouseWheelAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../scrolls/ScrollBarAction.h"
#include "../scrolls/VScrollBarUpClickAction.h"
#include "../scrolls/VScrollBarDownClickAction.h"

#pragma warning(disable:4996)

MouseWheelAction::MouseWheelAction(CWnd* parent, CPoint point, short zDelta)
	:MouseAction(parent, point) {
	this->zDelta = zDelta;
}

MouseWheelAction::~MouseWheelAction() {

}

void MouseWheelAction::Perform() {
	//1. 수직스크롤이 존재한다면,
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	BOOL hasVScroll = notepadForm->scrollController->HasVScroll();
	if (hasVScroll)
	{
		//1.1. 방향을 확인한다.
		BOOL isUp = TRUE;
		if (zDelta < 0)
		{
			isUp = FALSE;
			zDelta *= -1;
		}

		//1.2. 반복 횟수를 정한다.
		Long count = zDelta / DELTA_PER_TICK;

		//1.3. 방향에 따라 반복한다.
		ScrollBarAction* action = NULL;
		if (isUp)
		{
			action = new VScrollBarUpClickAction(this->parent);
		}
		else
		{
			action = new VScrollBarDownClickAction(this->parent);
		}

		for (Long i = 0; i < count; i++)
		{
			action->Perform();
		}

		if (action != NULL)
		{
			delete action;
		}
	}
}
