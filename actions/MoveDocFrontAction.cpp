#include <afxwin.h>
#include "MoveDocFrontAction.h"
#include "../PageManager.h"

#pragma warning(disable:4996)

MoveDocFrontAction::MoveDocFrontAction(CWnd* parent)
	:Action(parent) {

}

MoveDocFrontAction::~MoveDocFrontAction() {

}

void MoveDocFrontAction::Perform() {
	PageManager::LoadFirst(this->parent);
}
