#include "MouseActionFactory.h"
#include "NotepadForm.h"
#include "MouseHandler.h"
#include "DragUpAction.h"
#include "DragDownAction.h"
#include "DragLeftAction.h"
#include "DragRightAction.h"

#pragma warning(disable:4996)

MouseActionFactory::MouseActionFactory(CWnd* parent) {
	this->parent = parent;
}

MouseActionFactory::~MouseActionFactory() {

}

MouseAction* MouseActionFactory::Create(UINT nFlags, CPoint point) {
	MouseAction* mouseAction = NULL;
	MouseHandler* mouseHandler = ((NotepadForm*)(this->parent))->mouseHandler;
	if (nFlags == MK_LBUTTON && mouseHandler->OnDrag())
	{
		if (mouseHandler->IsMovedAboveRow(point))
		{
			mouseAction = new DragUpAction(this->parent, point);
		}
		else if (mouseHandler->IsMovedBelowRow(point))
		{
			mouseAction = new DragDownAction(this->parent, point);
		}
		else if (mouseHandler->IsMovedLeft(point))
		{
			mouseAction = new DragLeftAction(this->parent, point);
		}
		else if (mouseHandler->IsMovedRight(point))
		{
			mouseAction = new DragRightAction(this->parent, point);
		}
	}

	return mouseAction;
}