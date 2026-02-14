#include "ScrollBarActionFactory.h"
#include "VScrollBarUpClickAction.h"
#include "VScrollBarDownClickAction.h"
#include "VScrollBarPageUpAction.h"
#include "VScrollBarPageDownAction.h"
#include "VScrollBarDragAction.h"
#include "HScrollBarLeftClickAction.h"
#include "HScrollBarRightClickAction.h"
#include "HScrollBarPageLeftAction.h"
#include "HScrollBarPageRightAction.h"
#include "HScrollBarDragAction.h"

#pragma warning(disable:4996)

ScrollBarActionFactory::ScrollBarActionFactory() {

}

ScrollBarActionFactory::~ScrollBarActionFactory() {

}

ScrollBarAction* ScrollBarActionFactory::Create(CWnd *parent, int nBar, UINT nSBCode, UINT nPos) {
	ScrollBarAction* scrollBarAction = NULL;

	if (nBar == SB_VERT)
	{
		switch (nSBCode)
		{
		case SB_LINEUP:
			scrollBarAction = new VScrollBarUpClickAction(parent);
			break;
		case SB_LINEDOWN:
			scrollBarAction = new VScrollBarDownClickAction(parent);
			break;
		case SB_PAGEUP:
			scrollBarAction = new VScrollBarPageUpAction(parent);
			break;
		case SB_PAGEDOWN:
			scrollBarAction = new VScrollBarPageDownAction(parent);
			break;
		case SB_THUMBTRACK:
			scrollBarAction = new VScrollBarDragAction(parent, nPos);
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
			scrollBarAction = new HScrollBarLeftClickAction(parent);
			break;
		case SB_LINERIGHT:
			scrollBarAction = new HScrollBarRightClickAction(parent);
			break;
		case SB_PAGELEFT:
			scrollBarAction = new HScrollBarPageLeftAction(parent);
			break;
		case SB_PAGERIGHT:
			scrollBarAction = new HScrollBarPageRightAction(parent);
			break;
		case SB_THUMBTRACK:
			scrollBarAction = new HScrollBarDragAction(parent, nPos);
			break;
		default:
			break;
		}
	}

	return scrollBarAction;
}