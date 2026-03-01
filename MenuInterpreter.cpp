#include "MenuInterpreter.h"
#include "resource.h"

#pragma warning(disable:4996)

MenuInterpreter::MenuInterpreter() {

}

MenuInterpreter::~MenuInterpreter() {

}

BOOL MenuInterpreter::IsFindReplace(UINT nID) {
	BOOL isFindReplace = FALSE;

	switch (nID)
	{
	case ID_MENU_FINDPREVIOUS:
	case ID_MENU_FINDNEXT:
		isFindReplace = TRUE;
		break;
	default:
		break;
	}

	return isFindReplace;
}


BOOL MenuInterpreter::IsCommand(UINT nID) {
	BOOL isCommand = FALSE;

	switch (nID)
	{
	case ID_MENU_CUT:
	case ID_MENU_COPY:
	case ID_MENU_PASTE:
	case ID_MENU_DELETE:
		isCommand = TRUE;
		break;
	default:
		break;
	}

	return isCommand;
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
	case ID_MENU_SETPAGE:
		appID = AppID::ID_ACTION_SET_PAGE;
		break;
	case ID_MENU_PREVIEW:
		appID = AppID::ID_ACTION_PREVIEW;
		break;
	case ID_MENU_PRINT:
		appID = AppID::ID_ACTION_PRINT;
		break;
	case ID_MENU_FONT:
		appID = AppID::ID_ACTION_CHANGE_FONT;
		break;
	case ID_MENU_AUTOWRAP:
		appID = AppID::ID_ACTION_AUTOWRAP;
		break;
	case ID_MENU_UNDO:
		appID = AppID::ID_ACTION_UNDO;
		break;
	case ID_MENU_REDO:
		appID = AppID::ID_ACTION_REDO;
		break;
	case ID_MENU_CUT:
		appID = AppID::ID_COMMAND_CUT;
		break;
	case ID_MENU_COPY:
		appID = AppID::ID_ACTION_COPY;
		break;
	case ID_MENU_PASTE:
		appID = AppID::ID_COMMAND_PASTE;
		break;
	case ID_MENU_DELETE:
		appID = AppID::ID_COMMAND_ERASE;
		break;
	case ID_MENU_FIND:
		appID = AppID::ID_ACTION_OPEN_FIND_DIALOG;
		break;
	case ID_MENU_REPLACE:
		appID = AppID::ID_ACTION_OPEN_REPLACE_DIALOG;
		break;
	case ID_MENU_FINDPREVIOUS:
		appID = AppID::ID_ACTION_FIND_PREVIOUS;
		break;
	case ID_MENU_FINDNEXT:
		appID = AppID::ID_ACTION_FIND_NEXT;
		break;
	case ID_MENU_ZOOMIN:
		appID = AppID::ID_ACTION_ZOOM_IN;
		break;
	case ID_MENU_ZOOMOUT:
		appID = AppID::ID_ACTION_ZOOM_OUT;
		break;
	case ID_MENU_ZOOMRESET:
		appID = AppID::ID_ACTION_ZOOM_RESET;
		break;
	case ID_MENU_INFO:
		appID = AppID::ID_ACTION_ABOUT_NOTEPAD;
		break;
	default:
		break;
	}

	return appID;
}