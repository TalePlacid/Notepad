#include <afxwin.h>
#include "AutoWrapCommand.h"
#include "NotepadForm.h"
#include "resource.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

AutoWrapCommand::AutoWrapCommand(CWnd* parent)
	:Command(parent) {

}

AutoWrapCommand::~AutoWrapCommand() {

}

void AutoWrapCommand::Execute() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->isAutoWrapped = !notepadForm->isAutoWrapped;

	NoteWrapper noteWrapper(this->parent);
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);
	if (notepadForm->isAutoWrapped)
	{
		noteWrapper.Wrap(notepadForm->note);
		notepadForm->menu.CheckMenuItem(ID_MENU_AUTOWRAP, MF_CHECKED);
	}
	else
	{
		noteWrapper.Unwrap();
		notepadForm->menu.CheckMenuItem(ID_MENU_AUTOWRAP, MF_UNCHECKED);
	}
}