#include <afxwin.h>
#include "AutoWrapCommand.h"
#include "NotepadForm.h"
#include "NoteWrapper.h"
#include "resource.h"

#pragma warning(disable:4996)

AutoWrapCommand::AutoWrapCommand(CWnd* parent)
	:Command(parent) {

}

AutoWrapCommand::~AutoWrapCommand() {

}

void AutoWrapCommand::Execute() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	NoteWrapper noteWrapper(this->parent);
	UINT isChecked = notepadForm->menu.GetMenuState(ID_MENU_AUTOWRAP, MF_BYCOMMAND);

	if (isChecked == MF_UNCHECKED)
	{
		notepadForm->menu.CheckMenuItem(ID_MENU_AUTOWRAP, MF_CHECKED);
		noteWrapper.InsertDummyRows();
	}
	else if (isChecked == MF_CHECKED)
	{
		notepadForm->menu.CheckMenuItem(ID_MENU_AUTOWRAP, MF_UNCHECKED);
		noteWrapper.DeleteDummyRows();
	}

	this->parent->Invalidate();
}