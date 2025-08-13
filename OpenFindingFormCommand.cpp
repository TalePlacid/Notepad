#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenFindingFormCommand.h"
#include "NotepadForm.h"
#include "message.h"

#pragma warning(disable:4996)

OpenFindingFormCommand::OpenFindingFormCommand(CWnd* parent)
	:Command(parent) {

}

OpenFindingFormCommand::~OpenFindingFormCommand() {

}

void OpenFindingFormCommand::Execute() {
	if (!((NotepadForm*)(this->parent))->hasFindReplaceForm)
	{
		CFindReplaceDialog* findingForm = new CFindReplaceDialog;
		findingForm->Create(TRUE, NULL, NULL, 1, this->parent);

		((NotepadForm*)(this->parent))->hasFindReplaceForm = TRUE;
		PostMessage(this->parent->GetSafeHwnd(), WM_FINDREPLACE_CREATED, (WPARAM)findingForm, 0);
	}
}