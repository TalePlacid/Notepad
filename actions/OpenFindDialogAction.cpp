#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenFindDialogAction.h"
#include "../message.h"
#include "../NotepadForm.h"
#include "../SearchResultController.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

OpenFindDialogAction::OpenFindDialogAction(CWnd* parent)
	:Action(parent) {

}

OpenFindDialogAction::~OpenFindDialogAction() {

}

void OpenFindDialogAction::Perform() {
	if (((NotepadForm*)(this->parent))->findReplaceDialog == NULL)
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

		((NotepadForm*)(this->parent))->findReplaceDialog = new CFindReplaceDialog;
		CFindReplaceDialog* findDialog = ((NotepadForm*)(this->parent))->findReplaceDialog;
		findDialog->Create(TRUE, (LPCTSTR)findString, NULL, 1, this->parent);

		PostMessage(this->parent->GetSafeHwnd(), WM_FINDREPLACE_FOCUS, (WPARAM)findDialog, 0);
	}
}