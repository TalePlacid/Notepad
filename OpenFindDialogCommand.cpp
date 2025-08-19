#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenFindDialogCommand.h"
#include "NotepadForm.h"
#include "message.h"
#include "SearchResultController.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

OpenFindDialogCommand::OpenFindDialogCommand(CWnd* parent)
	:Command(parent) {

}

OpenFindDialogCommand::~OpenFindDialogCommand() {

}

void OpenFindDialogCommand::Execute() {
	if (!((NotepadForm*)(this->parent))->hasFindReplaceDialog)
	{
		CString key("");
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
		if (pagingBuffer->GetSelectionBeginOffset() >= 0)
		{
			key = pagingBuffer->MakeSelectedString();
		}
		else if (searchResultController != NULL)
		{
			key = CString(searchResultController->GetKey().c_str());
		}

		CFindReplaceDialog* findDialog = new CFindReplaceDialog;
		findDialog->Create(TRUE, (LPCTSTR)key, NULL, 1, this->parent);

		((NotepadForm*)(this->parent))->hasFindReplaceDialog = TRUE;
		PostMessage(this->parent->GetSafeHwnd(), WM_FINDREPLACE_FOCUS, (WPARAM)findDialog, 0);
	}
}