#include <afxwin.h>
#include "MoveDocRearAction.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

MoveDocRearAction::MoveDocRearAction(CWnd* parent)
	:Action(parent) {

}

MoveDocRearAction::~MoveDocRearAction() {

}

void MoveDocRearAction::Perform() {
	PageManager::LoadLast(this->parent);
}
