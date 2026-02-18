#include "MenuInterpreter.h"
#include "resource.h"

#pragma warning(disable:4996)

MenuInterpreter::MenuInterpreter() {

}

MenuInterpreter::~MenuInterpreter() {

}

BOOL MenuInterpreter::IsAction(UINT nID) {
	BOOL isAction = TRUE;
	if (nID == ID_MENU_UNDO)
	{
		isAction = FALSE;
	}

	return isAction;
}

AppID MenuInterpreter::DetermineID(UINT nID){
	AppID appID = AppID::NONE;

	switch (nID)
	{
	case ID_MENU_NEW:
		appID = AppID::ID_ACTION_NEW;
		break;
	case ID_MENU_NEWWINDOW:
		appID = AppID::ID_ACTION_NEW_WINDOW;
		break;
	case ID_MENU_OPEN:
		appID = AppID::ID_ACTION_OPEN;
		break;
	case ID_MENU_SAVE:
		appID = AppID::ID_ACTION_SAVE;
		break;
	case ID_MENU_SAVEAS:
		appID = AppID::ID_ACTION_SAVE_AS;
		break;
	case ID_MENU_FONT:
		appID = AppID::ID_ACTION_CHANGE_FONT;
		break;
	default:
		break;
	}

	return appID;
}