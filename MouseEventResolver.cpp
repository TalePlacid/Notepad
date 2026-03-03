#include "MouseEventResolver.h"

#pragma warning(disable:4996)

MouseEventResolver::MouseEventResolver() {

}

MouseEventResolver::~MouseEventResolver() {

}

AppID MouseEventResolver::Resolve(AppID rawID, UINT nFlags, CPoint point, short zDelta) {
	AppID appID = AppID::NONE;
	switch (rawID)
	{
	case AppID::ID_MOUSE_LBUTTON_DOWN:
		appID = AppID::ID_ACTION_MOVE_TO_POINT;
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
		break;
	default:
		break;
	}

	return appID;
}