#include <afxwin.h>
#include "CtrlHomeAction.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

CtrlHomeAction::CtrlHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlHomeAction::~CtrlHomeAction() {

}

void CtrlHomeAction::Perform() {
	PageLoader::LoadFirst(this->parent);
}