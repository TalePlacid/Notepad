#include "MouseCtrlWheelAction.h"
#include "../resource.h"

#pragma warning(disable:4996)

MouseCtrlWheelAction::MouseCtrlWheelAction(CWnd* parent, CPoint point, short zDelta)
	:MouseAction(parent, point) {
	this->zDelta = zDelta;
}

MouseCtrlWheelAction::~MouseCtrlWheelAction() {

}

void MouseCtrlWheelAction::Perform() {
	//1. 방향을 확인한다.
#if 0
	UINT nID = ID_ACTION_ZOOMIN;
	if (zDelta < 0)
	{
		nID = ID_ACTION_ZOOMOUT;
		zDelta *= -1;
	}
#endif
	//2. 반복 횟수를 정한다.
	Long count = zDelta / DELTA_PER_TICK;

	//3. 방향에 따라 반복한다.
	for (Long i = 0; i < count; i++)
	{
		//SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)nID, 0);
	}
}
