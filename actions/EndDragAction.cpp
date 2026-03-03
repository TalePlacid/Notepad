#include "EndDragAction.h"
#include "../NotepadForm.h"
#include "../MouseHandler.h"

#pragma warning(disable:4996)

EndDragAction::EndDragAction(CWnd* parent)
	:Action(parent) {

}

EndDragAction::~EndDragAction() {

}

void EndDragAction::Perform() {
	MouseHandler* mouseHandler = ((NotepadForm*)(this->parent))->mouseHandler;
	mouseHandler->EndDrag();
}
