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
#include "CtrlCAction.h"
#include "CtrlVAction.h"
#include "PageUpAction.h"
#include "PageDownAction.h"
#include "CtrlXAction.h"
#include "CtrlFAction.h"
#include "CtrlHAction.h"
#include "CtrlZAction.h"
#include "CtrlYAction.h"
#include "ShiftPageDownAction.h"
#include "ShiftPageUpAction.h"
#include "CtrlPageDownAction.h"
#include "CtrlPageUpAction.h"
#include "CtrlAAction.h"
#include "ZoomInAction.h"
#include "ZoomOutAction.h"
#include "ZoomResetAction.h"

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
	case VK_PRIOR:
		if (onShiftKey)
		{
			keyAction = new ShiftPageUpAction(parent);
		}
		else if (onCtrlKey)
		{
			keyAction = new CtrlPageUpAction(parent);
		}
		else
		{
			keyAction = new PageUpAction(parent);
		}
		break;
	case VK_NEXT:
		if (onShiftKey)
		{
			keyAction = new ShiftPageDownAction(parent);
		}
		else if (onCtrlKey)
		{
			keyAction = new CtrlPageDownAction(parent);
		}
		else
		{
			keyAction = new PageDownAction(parent);
		}
		break;
	case VK_OEM_PLUS:
	case VK_ADD:
		if (onCtrlKey)
		{
			keyAction = new ZoomInAction(parent);
		}
		break;
	case VK_OEM_MINUS:
	case VK_SUBTRACT:
		if (onCtrlKey)
		{
			keyAction = new ZoomOutAction(parent);
		}
		break;
	case VK_NUMPAD0:
	case '0':
		if (onCtrlKey)
		{
			keyAction = new ZoomResetAction(parent);
		}
		break;
	case 'A':
		if (onCtrlKey)
		{
			keyAction = new CtrlAAction(parent);
		}
		break;
	case 'C':
		if (onCtrlKey)
		{
			keyAction = new CtrlCAction(parent);
		}
		break;
	case 'F':
		if (onCtrlKey)
		{
			keyAction = new CtrlFAction(parent);
		}
		break;
	case 'H':
		if (onCtrlKey)
		{
			keyAction = new CtrlHAction(parent);
		}
		break;
	case 'X':
		if (onCtrlKey)
		{
			keyAction = new CtrlXAction(parent);
		}
		break;
	case 'V':
		if (onCtrlKey)
		{
			keyAction = new CtrlVAction(parent);
		}
		break;
	case 'Y':
		if (onCtrlKey)
		{
			keyAction = new CtrlYAction(parent);
		}
		break;
	case 'Z':
		if (onCtrlKey && onShiftKey)
		{
			keyAction = new CtrlYAction(parent);
		}
		else if (onCtrlKey)
		{
			keyAction = new CtrlZAction(parent);
		}
		break;
	default:
		break;
	}

	return keyAction;
}