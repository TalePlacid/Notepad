#include "DragAction.h"
#include "../NotepadForm.h"
#include "../MouseHandler.h"
#include "../Editor.h"

#pragma warning(disable:4996)

DragAction::DragAction(CWnd* parent, CPoint point)
	:Action(parent), point(point) {

}

DragAction::~DragAction() {

}

void DragAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	MouseHandler* mouseHandler = notepadForm->mouseHandler;
	Direction direction = mouseHandler->CheckDirection(this->point);
	Editor editor(this->parent);

	//1. 방향에 맞는 에디터 드래그 메서드를 실행한다.
	switch (direction)
	{
	case DIRECTION_UP:
		editor.DragUp(this->point);
		break;
	case DIRECTION_DOWN:
		editor.DragDown(this->point);
		break;
	case DIRECTION_LEFT:
		editor.DragLeft(this->point);
		break;
	case DIRECTION_RIGHT:
		editor.DragRight(this->point);
		break;
	default:
		break;
	}
}