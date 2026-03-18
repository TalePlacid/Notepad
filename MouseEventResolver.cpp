#include "MouseEventResolver.h"

#pragma warning(disable:4996)

MouseEventResolver::MouseEventResolver() {

}

MouseEventResolver::~MouseEventResolver() {

}

AppID MouseEventResolver::Resolve(AppID rawID, UINT nFlags, CPoint point, BOOL isDragging, short zDelta) {
	AppID appID = AppID::NONE;
	switch (rawID)
	{
	case AppID::ID_MOUSE_LBUTTON_DOWN:
		appID = AppID::ID_ACTION_MOVE_TO_POINT;
		break;
	case AppID::ID_MOUSE_MOVE:
		if (nFlags == MK_LBUTTON)
		{
			appID = AppID::ID_ACTION_DRAG;
		}
		break;
	case AppID::ID_MOUSE_LBUTTON_UP:
		appID = AppID::ID_ACTION_END_DRAG;
		break;
	case AppID::ID_MOUSE_WHEEL:
		if (nFlags == MK_CONTROL)
		{
			appID = AppID::ID_ACTION_ZOOM_IN;
			if (zDelta < 0)
			{
				appID = AppID::ID_ACTION_ZOOM_OUT;
			}
		}
		else if (isDragging)
		{
			appID = AppID::ID_ACTION_DRAG_WHEEL_UP;
			if (zDelta < 0)
			{
				appID = AppID::ID_ACTION_DRAG_WHEEL_DOWN;
			}
		}
		else
		{
			appID = AppID::ID_ACTION_VSCROLL_LINE_UP;
			if (zDelta < 0)
			{
				appID = AppID::ID_ACTION_VSCROLL_LINE_DOWN;
			}
		}
		break;
	default:
		break;
	}

	return appID;
}
