#include <afxdlgs.h>
#include "FindReplaceCommandFactory.h"
#include "Command.h"
#include "FindCommand.h"
#include "FindNextCommand.h"
#include "CloseFindReplaceCommand.h"
#include "SearchResultController.h"
#include "NotepadForm.h"
#include "FindNextCommand_Fixed.h"

#pragma warning(disable:4996)

FindReplaceCommandFactory::FindReplaceCommandFactory() {

}

FindReplaceCommandFactory::~FindReplaceCommandFactory() {

}

Command* FindReplaceCommandFactory::Create(CWnd* parent, CFindReplaceDialog* findReplaceDialog) {
	Command* command = NULL;

	FindReplaceOption findReplaceOption(findReplaceDialog->GetFindString(),
		findReplaceDialog->MatchWholeWord(), findReplaceDialog->MatchCase(), findReplaceDialog->SearchDown());

	SearchResultController* searchResultController = ((NotepadForm*)parent)->searchResultController;
	FindReplaceOption searchedOption(CString(searchResultController->GetKey().c_str()), searchResultController->IsMatchWhole(), searchResultController->IsMatchCase(), searchResultController->IsSearchDown());

	if (findReplaceDialog->IsTerminating())
	{
		command = new CloseFindReplaceCommand(parent, findReplaceDialog);
	}
	else if (findReplaceOption != searchedOption)
	{
		command = new FindCommand(parent, findReplaceDialog);
	}
	else if (findReplaceOption == searchedOption)
	{
		command = new FindNextCommand_Fixed(parent, findReplaceDialog);
	}

	return command;
}