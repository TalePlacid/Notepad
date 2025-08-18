#include <afxwin.h>
#include <afxdlgs.h>
#include "CloseFindReplaceCommand.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

CloseFindReplaceCommand::CloseFindReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:FindReplaceCommand(parent, findReplaceDialog) {

}

CloseFindReplaceCommand::~CloseFindReplaceCommand() {

}

void CloseFindReplaceCommand::Execute() {
	((NotepadForm*)(this->parent))->hasFindReplaceForm = FALSE;
}