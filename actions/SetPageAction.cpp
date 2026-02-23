#include <afxwin.h>
#include "SetPageAction.h"
#include "../prints/PageSettingForm.h"

#pragma warning(disable:4996)

SetPageAction::SetPageAction(CWnd* parent)
	:Action(parent) {

}

SetPageAction::~SetPageAction() {

}

void SetPageAction::Perform() {
	PageSettingForm pageSettingForm(this->parent);
	pageSettingForm.DoModal();
}