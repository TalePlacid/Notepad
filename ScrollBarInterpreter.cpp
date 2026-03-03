#include "ScrollBarInterpreter.h"

#pragma warning(disable:4996)

ScrollBarInterpreter::ScrollBarInterpreter() {

}

ScrollBarInterpreter::~ScrollBarInterpreter() {

}

AppID ScrollBarInterpreter::DetermineID(int nBar, UINT nSBCode) {
	AppID nID = AppID::NONE;

	if (nBar == SB_VERT)
	{
		switch (nSBCode)
		{
		case SB_LINEUP:
			nID = AppID::ID_ACTION_VSCROLL_LINE_UP;
			break;
		case SB_LINEDOWN:
			nID = AppID::ID_ACTION_VSCROLL_LINE_DOWN;
			break;
		case SB_PAGEUP:
			nID = AppID::ID_ACTION_VSCROLL_PAGE_UP;
			break;
		case SB_PAGEDOWN:
			nID = AppID::ID_ACTION_VSCROLL_PAGE_DOWN;
			break;
		case SB_THUMBTRACK:
			nID = AppID::ID_ACTION_VSCROLL_THUMB_TRACK;
			break;
		default:
			break;
		}
	}
	else if (nBar == SB_HORZ)
	{
		switch (nSBCode)
		{
		case SB_LINELEFT:
			nID = AppID::ID_ACTION_HSCROLL_LINE_LEFT;
			break;
		case SB_LINERIGHT:
			nID = AppID::ID_ACTION_HSCROLL_LINE_RIGHT;
			break;
		case SB_PAGELEFT:
			nID = AppID::ID_ACTION_HSCROLL_PAGE_LEFT;
			break;
		case SB_PAGERIGHT:
			nID = AppID::ID_ACTION_HSCROLL_PAGE_RIGHT;
			break;
		case SB_THUMBTRACK:
			nID = AppID::ID_ACTION_HSCROLL_THUMB_TRACK;
			break;
		default:
			break;
		}
	}

	return nID;
}
