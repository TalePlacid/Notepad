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
	default:
		break;
	}

	return appID;
}