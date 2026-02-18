#include "KeyDownInterpreter.h"

#pragma warning(disable:4996)

KeyDownInterpreter::KeyDownInterpreter() {

}

KeyDownInterpreter::~KeyDownInterpreter() {

}

BOOL KeyDownInterpreter::IsAction(UINT nChar) {
	BOOL isAction = TRUE;

	if (nChar == VK_BACK)
	{
		isAction = FALSE;
	}

	return isAction;
}

AppID KeyDownInterpreter::DetermineID(UINT nChar) {
	AppID nID = AppID::NONE;

	int onCtrlKey = GetKeyState(VK_CONTROL) & 0x8000;
	int onShiftKey = GetKeyState(VK_SHIFT) & 0x8000;

	switch (nChar)
	{
	case VK_BACK:
		nID = AppID::ID_COMMAND_ERASE;
		break;
	case 'N':
		if (onCtrlKey && onShiftKey)
		{
			nID = AppID::ID_ACTION_NEW_WINDOW;
		}
		else if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_NEW;
		}
		break;
	case 'O':
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_OPEN;
		}
		break;
	case 'S':
		if (onCtrlKey && onShiftKey)
		{
			nID = AppID::ID_ACTION_SAVE_AS;
		}
		else if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_SAVE;
		}
		break;
	default:
		break;
	}

	return nID;
}