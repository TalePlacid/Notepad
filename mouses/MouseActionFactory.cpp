#include "MouseActionFactory.h"
#include "MouseLeftDownAction.h"
#include "MouseWheelAction.h"
#include "MouseCtrlWheelAction.h"
#include "DragUpAction.h"
#include "DragDownAction.h"
#include "DragLeftAction.h"
#include "DragRightAction.h"
#include "../NotepadForm.h"
#include "../MouseHandler.h"

#pragma warning(disable:4996)

MouseActionFactory::MouseActionFactory(CWnd* parent) {
	this->parent = parent;
}

MouseActionFactory::~MouseActionFactory() {

}

MouseAction* MouseActionFactory::Create(UINT nID, UINT nFlags, CPoint point, short zDelta) {
	MouseAction* mouseAction = NULL;
	switch (nID)
	{
	case WM_LBUTTONDOWN:
		mouseAction = new MouseLeftDownAction(this->parent, point);
		break;
	case WM_MOUSEWHEEL:
		switch (nFlags)
		{
		case MK_CONTROL:
			mouseAction = new MouseCtrlWheelAction(this->parent, point, zDelta);
			break;
		default:
			mouseAction = new MouseWheelAction(this->parent, point, zDelta);
			break;
		}
		break;
	case WM_MOUSEMOVE:
	{
		MouseHandler* mouseHandler = ((NotepadForm*)(this->parent))->mouseHandler;
		Direction direction = mouseHandler->CheckDirection(point);
		switch (direction)
		{
		case DIRECTION_UP:
			mouseAction = new DragUpAction(this->parent, point);
			break;
		case DIRECTION_DOWN:
			mouseAction = new DragDownAction(this->parent, point);
			break;
		case DIRECTION_LEFT:
			mouseAction = new DragLeftAction(this->parent, point);
			break;
		case DIRECTION_RIGHT:
			mouseAction = new DragRightAction(this->parent, point);
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}

	return mouseAction;
}