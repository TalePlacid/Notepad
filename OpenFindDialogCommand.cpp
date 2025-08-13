#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenFindDialogCommand.h"
#include "NotepadForm.h"
#include "message.h"
#include "SearchResultController.h"

#pragma warning(disable:4996)

OpenFindDialogCommand::OpenFindDialogCommand(CWnd* parent)
	:Command(parent) {

}

OpenFindDialogCommand::~OpenFindDialogCommand() {

}

void OpenFindDialogCommand::Execute() {
	if (!((NotepadForm*)(this->parent))->hasFindReplaceForm)
	{
		CString key("");
		SearchResultController *searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
		if (searchResultController != NULL)
		{
			key = CString(searchResultController->GetKey().c_str());
		}

		CFindReplaceDialog* findingForm = new CFindReplaceDialog;
		findingForm->Create(TRUE, (LPCTSTR)key, NULL, 1, this->parent);

		((NotepadForm*)(this->parent))->hasFindReplaceForm = TRUE;
		PostMessage(this->parent->GetSafeHwnd(), WM_FINDREPLACE_CREATED, (WPARAM)findingForm, 0);
	}
}