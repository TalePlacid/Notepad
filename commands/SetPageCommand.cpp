#include <afxwin.h>
#include "SetPageCommand.h"
#include "../prints/PageSettingForm.h"

#pragma warning(disable:4996)

SetPageCommand::SetPageCommand(CWnd* parent)
	:Command(parent) {

}

SetPageCommand::~SetPageCommand() {

}

void SetPageCommand::Execute() {
	PageSettingForm pageSettingForm(this->parent);
	pageSettingForm.DoModal();
}