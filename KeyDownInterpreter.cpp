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
	case VK_UP:
		nID = AppID::ID_ACTION_MOVE_UP;
		break;
	case VK_DOWN:
		nID = AppID::ID_ACTION_MOVE_DOWN;
		break;
	case VK_LEFT:
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_WORD_LEFT;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_LEFT;
		}
		break;
	case VK_RIGHT:
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_WORD_RIGHT;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_RIGHT;
		}
		break;
	case VK_HOME:
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_DOC_FRONT;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_LINE_FRONT;
		}
		break;
	case VK_END:
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_DOC_REAR;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_LINE_REAR;
		}
		break;
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