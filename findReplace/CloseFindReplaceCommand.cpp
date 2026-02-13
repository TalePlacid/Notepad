#include <afxwin.h>
#include <afxdlgs.h>
#include "CloseFindReplaceCommand.h"
#include "SearchResultController.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

CloseFindReplaceCommand::CloseFindReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

CloseFindReplaceCommand::~CloseFindReplaceCommand() {

}

void CloseFindReplaceCommand::Execute() {
	((NotepadForm*)(this->parent))->hasFindReplaceDialog = FALSE;
	((NotepadForm*)(this->parent))->searchResultController->ChangeFindReplaceOption(FindReplaceOption());
}