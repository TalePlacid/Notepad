#include "ToggleStatusBarAction.h"
#include "../NotepadForm.h"
#include "../NotepadFrame.h"
#include "../StatusBarController.h"
#include "../resource.h"

#pragma warning(disable:4996)

ToggleStatusBarAction::ToggleStatusBarAction(CWnd* parent)
	:Action(parent) {

}

ToggleStatusBarAction::~ToggleStatusBarAction() {

}

void ToggleStatusBarAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	NotepadFrame* notepadFrame = (NotepadFrame*)(notepadForm->GetParent());
	StatusBarController* statusBarController = notepadFrame->GetStatusBarController();
	if (statusBarController != NULL)
	{
		BOOL isVisible = statusBarController->ToggleVisible();
		notepadFrame->RecalcLayout();
		notepadFrame->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDC_NOTEPADFORM);

		CMenu* menu = notepadFrame->GetMenu();
		if (menu != NULL)
		{
			UINT checkState = MF_BYCOMMAND;
			if (isVisible)
			{
				checkState |= MF_CHECKED;
			}
			else
			{
				checkState |= MF_UNCHECKED;
			}
			menu->CheckMenuItem(ID_MENU_STATUSBAR, checkState);
		}
	}
}
