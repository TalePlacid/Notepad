#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenReplaceDialogCommand.h"
#include "../message.h"
#include "../NotepadForm.h"
#include "../SearchResultController.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

OpenReplaceDialogCommand::OpenReplaceDialogCommand(CWnd* parent)
	:Command(parent) {

}

OpenReplaceDialogCommand::~OpenReplaceDialogCommand() {

}

void OpenReplaceDialogCommand::Execute() {
	if (!((NotepadForm*)(this->parent))->hasFindReplaceDialog)
	{
		CString findString("");
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
		if (pagingBuffer->GetSelectionBeginOffset() >= 0)
		{
			findString = pagingBuffer->MakeSelectedString();
		}
		else if (searchResultController != NULL)
		{
			findString = searchResultController->GetFindReplaceOption().findString;
		}

		CFindReplaceDialog* replaceDialog = new CFindReplaceDialog;
		replaceDialog->Create(FALSE, (LPCTSTR)findString, NULL, 1, this->parent);

		((NotepadForm*)(this->parent))->hasFindReplaceDialog = TRUE;
		PostMessage(this->parent->GetSafeHwnd(), WM_FINDREPLACE_FOCUS, (WPARAM)replaceDialog, 0);
	}
}