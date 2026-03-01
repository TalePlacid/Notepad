#include <afxwin.h>
#include "MoveDownAction.h"
#include "../Editor.h"

#pragma warning(disable:4996)

MoveDownAction::MoveDownAction(CWnd* parent)
	:Action(parent) {

}

MoveDownAction::~MoveDownAction() {

}

void MoveDownAction::Perform() {
	Editor editor(this->parent);
	editor.MoveDown();
}
