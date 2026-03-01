#include <afxwin.h>
#include "MoveDocRearAction.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

MoveDocRearAction::MoveDocRearAction(CWnd* parent)
	:Action(parent) {

}

MoveDocRearAction::~MoveDocRearAction() {

}

void MoveDocRearAction::Perform() {
	PageLoader::LoadLast(this->parent);
}
