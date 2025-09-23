#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenReplaceDialogCommand.h"
#include "NotepadForm.h"
#include "SearchResultController.h"
#include "PagingBuffer.h"
#include "message.h"

#pragma warning(disable:4996)

OpenReplaceDialogCommand::OpenReplaceDialogCommand(CWnd* parent)
	:Command(parent) {

}

OpenReplaceDialogCommand::~OpenReplaceDialogCommand() {

}

void OpenReplaceDialogCommand::Execute() {
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

		CFindReplaceDialog* replaceDialog = new CFindReplaceDialog;
		replaceDialog->Create(FALSE, (LPCTSTR)key, NULL, 1, this->parent);

		if (searchResultController != NULL)
		{
			delete searchResultController;
			((NotepadForm*)(this->parent))->searchResultController = NULL;
		}

		((NotepadForm*)(this->parent))->hasFindReplaceDialog = TRUE;
		PostMessage(this->parent->GetSafeHwnd(), WM_FINDREPLACE_FOCUS, (WPARAM)replaceDialog, 0);
	}
}