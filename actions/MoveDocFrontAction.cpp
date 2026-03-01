#include <afxwin.h>
#include "MoveDocFrontAction.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

MoveDocFrontAction::MoveDocFrontAction(CWnd* parent)
	:Action(parent) {

}

MoveDocFrontAction::~MoveDocFrontAction() {

}

void MoveDocFrontAction::Perform() {
	PageLoader::LoadFirst(this->parent);
}
