#include <afxwin.h>
#include "MoveUpAction.h"
#include "../Editor.h"

#pragma warning(disable:4996)

MoveUpAction::MoveUpAction(CWnd* parent)
	:Action(parent) {

}

MoveUpAction::~MoveUpAction() {

}

void MoveUpAction::Perform() {
	Editor editor(this->parent);
	editor.MoveUp();
}
