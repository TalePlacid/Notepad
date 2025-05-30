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
#include "ShiftLeftAction.h"
#include "ShiftRightAction.h"
#include "ShiftUpAction.h"
#include "ShiftDownAction.h"
#include "ShiftHomeAction.h"
#include "ShiftEndAction.h"
#include "CtrlShiftLeftAction.h"
#include "CtrlShiftHomeAction.h"
#include "CtrlShiftEndAction.h"
#include "CtrlShiftRightAction.h"
#include "CtrlTAction.h"
#include "CtrlShiftTAction.h"

#pragma warning(disable:4996)

KeyActionFactory::KeyActionFactory() {

}

KeyActionFactory::~KeyActionFactory() {

}

KeyAction* KeyActionFactory::Create(CWnd* parent, UINT nChar) {
	KeyAction* keyAction = NULL;

	int onCtrlKey = GetKeyState(VK_CONTROL) & 0x8000;
	int onShiftKey = GetKeyState(VK_SHIFT) & 0x8000;

	switch (nChar)
	{
	case VK_LEFT:
		if (onCtrlKey && onShiftKey)
		{
			keyAction = new CtrlShiftLeftAction(parent);
		}
		else if	(onCtrlKey)
		{
			keyAction = new CtrlLeftAction(parent);
		}
		else if (onShiftKey)
		{
			keyAction = new ShiftLeftAction(parent);
		}
		else
		{
			keyAction = new LeftArrowAction(parent);
		}
		break;
	case VK_RIGHT:
		if (onCtrlKey && onShiftKey)
		{
			keyAction = new CtrlShiftRightAction(parent);
		}
		else if (onCtrlKey)
		{
			keyAction = new CtrlRightAction(parent);
		}
		else if (onShiftKey)
		{
			keyAction = new ShiftRightAction(parent);
		}
		else
		{
			keyAction = new RightArrowAction(parent);
		}
		break;
	case VK_UP:
		if (onShiftKey)
		{
			keyAction = new ShiftUpAction(parent);
		}
		else
		{
			keyAction = new UpArrowAction(parent);
		}
		break;
	case VK_DOWN:
		if (onShiftKey)
		{
			keyAction = new ShiftDownAction(parent);
		}
		else
		{
			keyAction = new DownArrowAction(parent);
		}
		break;
	case VK_HOME:
		if (onCtrlKey && onShiftKey)
		{
			keyAction = new CtrlShiftHomeAction(parent);
		}
		else if(onCtrlKey)
		{
			keyAction = new CtrlHomeAction(parent);
		}
		else if (onShiftKey)
		{
			keyAction = new ShiftHomeAction(parent);
		}
		else
		{
			keyAction = new HomeAction(parent);
		}
		break;
	case VK_END:
		if (onCtrlKey && onShiftKey)
		{
			keyAction = new CtrlShiftEndAction(parent);
		}
		else if (onCtrlKey)
		{
			keyAction = new CtrlEndAction(parent);
		}
		else if (onShiftKey)
		{
			keyAction = new ShiftEndAction(parent);
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
		if (onCtrlKey && onShiftKey)
		{
			keyAction = new CtrlShiftTAction(parent);
		}
		else if (onCtrlKey)
		{
			keyAction = new CtrlTAction(parent);
		}
		break;
	default:
		break;
	}

	return keyAction;
}