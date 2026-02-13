#include <afxwin.h>
#include "MouseAction.h"

#pragma warning(disable:4996)

MouseAction::MouseAction(CWnd* parent, CPoint point)
	:Action(parent) {
	this->point = point;
}

MouseAction::~MouseAction() {

}