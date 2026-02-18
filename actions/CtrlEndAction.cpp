#include <afxwin.h>
#include "CtrlEndAction.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

CtrlEndAction::CtrlEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlEndAction::~CtrlEndAction() {

}

void CtrlEndAction::Perform() {
	PageLoader::LoadLast(this->parent);
}