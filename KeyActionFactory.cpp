#include "KeyActionFactory.h"
#include "LeftArrowAction.h"
#include "RightArrowAction.h"
#include "UpArrowAction.h"
#include "DownArrowAction.h"
#include "HomeAction.h"
#include "EndAction.h"
#include "CtrlLeftAction.h"
#include "CtrlRightAction.h"
#include "CtrlHomeAction.h"
#include "CtrlEndAction.h"
#include "BackspaceAction.h"
#include "CtrlTAction.h"
#include "CtrlShiftTAction.h"

#pragma warning(disable:4996)

KeyActionFactory::KeyActionFactory() {

}

KeyActionFactory::~KeyActionFactory() {

}

KeyAction* KeyActionFactory::Create(CWnd* parent, UINT nChar) {
	KeyAction* keyAction = NULL;
	switch (nChar)
	{
	case VK_LEFT:
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			keyAction = new CtrlLeftAction(parent);
		}
		else
		{
			keyAction = new LeftArrowAction(parent);
		}
		break;
	case VK_RIGHT:
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			keyAction = new CtrlRightAction(parent);
		}
		else
		{
			keyAction = new RightArrowAction(parent);
		}
		break;
	case VK_UP:
		keyAction = new UpArrowAction(parent);
		break;
	case VK_DOWN:
		keyAction = new DownArrowAction(parent);
		break;
	case VK_HOME:
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			keyAction = new CtrlHomeAction(parent);
		}
		else
		{
			keyAction = new HomeAction(parent);
		}
		break;
	case VK_END:
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			keyAction = new CtrlEndAction(parent);
		}
		else
		{
			keyAction = new EndAction(parent);
		}
		break;
	case VK_BACK:
		keyAction = new BackspaceAction(parent);
		break;
	case 'T':
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) &0x8000)
			{
				keyAction = new CtrlShiftTAction(parent);
			}
			else
			{
				keyAction = new CtrlTAction(parent);
			}
		}
		break;
	default:
		break;
	}

	return keyAction;
}