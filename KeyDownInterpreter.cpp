#include "KeyDownInterpreter.h"

#pragma warning(disable:4996)

KeyDownInterpreter::KeyDownInterpreter() {

}

KeyDownInterpreter::~KeyDownInterpreter() {

}

BOOL KeyDownInterpreter::IsAction(UINT nChar) {
	BOOL isAction = TRUE;

	int onCtrlKey = GetKeyState(VK_CONTROL) & 0x8000;
	int onShiftKey = GetKeyState(VK_SHIFT) & 0x8000;

	switch (nChar)
	{
	case VK_BACK:
		isAction = FALSE;
		break;
	case 'V':
		if (onCtrlKey)
		{
			isAction = FALSE;
		}
		break;
	default:
		break;
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
		if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_UP;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_UP;
		}
		break;
	case VK_DOWN:
		if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_DOWN;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_DOWN;
		}
		break;
	case VK_LEFT:
		if (onCtrlKey && onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_WORD_LEFT;
		}
		else if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_WORD_LEFT;
		}
		else if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_LEFT;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_LEFT;
		}
		break;
	case VK_RIGHT:
		if (onCtrlKey && onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_WORD_RIGHT;
		}
		else if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_WORD_RIGHT;
		}
		else if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_RIGHT;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_RIGHT;
		}
		break;
	case VK_HOME:
		if (onCtrlKey && onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_DOC_FRONT;
		}
		else if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_DOC_FRONT;
		}
		else if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_LINE_FRONT;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_LINE_FRONT;
		}
		break;
	case VK_END:
		if (onCtrlKey && onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_DOC_REAR;
		}
		else if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_MOVE_DOC_REAR;
		}
		else if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_LINE_REAR;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_LINE_REAR;
		}
		break;
	case VK_PRIOR:
		if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_PAGE_UP;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_PAGE_UP;
		}
		break;
	case VK_NEXT:
		if (onShiftKey)
		{
			nID = AppID::ID_ACTION_SELECT_PAGE_DOWN;
		}
		else
		{
			nID = AppID::ID_ACTION_MOVE_PAGE_DOWN;
		}
		break;
	case VK_BACK:
		nID = AppID::ID_COMMAND_ERASE;
		break;
	case 'A':
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_SELECT_ALL;
		}
		break;
	case 'C':
		if (onCtrlKey)
		{
			nID = AppID::ID_ACTION_COPY;
		}
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
	case 'V':
		if (onCtrlKey)
		{
			nID = AppID::ID_COMMAND_PASTE;
		}
	default:
		break;
	}

	return nID;
}