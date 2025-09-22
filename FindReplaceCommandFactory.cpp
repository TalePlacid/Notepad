#include <afxdlgs.h>
#include "FindReplaceCommandFactory.h"
#include "NotepadForm.h"
#include "FindReplaceOption.h"
#include "Command.h"
#include "SearchResultController.h"
#include "PagingBuffer.h"
#include "CommandFactory.h"
#include "resource.h"

#pragma warning(disable:4996)

FindReplaceCommandFactory::FindReplaceCommandFactory() {

}

FindReplaceCommandFactory::~FindReplaceCommandFactory() {

}

Command* FindReplaceCommandFactory::Create(CWnd* parent, CFindReplaceDialog* findReplaceDialog) {
	CommandFactory commandFactory;
	Command* command = NULL;

	FindReplaceOption findReplaceOption(findReplaceDialog->GetFindString(),
		findReplaceDialog->MatchWholeWord(), findReplaceDialog->MatchCase(), findReplaceDialog->SearchDown());

	SearchResultController* searchResultController = ((NotepadForm*)parent)->searchResultController;
	FindReplaceOption searchedOption(CString(searchResultController->GetKey().c_str()), searchResultController->IsMatchWhole(), searchResultController->IsMatchCase(), searchResultController->IsSearchDown());

	PagingBuffer* pagingBuffer = ((NotepadForm*)parent)->pagingBuffer;
	UINT nID = 0;
	if (findReplaceDialog->IsTerminating())
	{
		nID = ID_COMMAND_CLOSEFINDREPLACE;
	}
	else if (findReplaceDialog->ReplaceAll())
	{
		nID = ID_COMMAND_REPLACEALL;
	}
	else if (findReplaceDialog->ReplaceCurrent())
	{
		if (findReplaceOption != searchedOption)
		{
			nID = ID_COMMAND_FIND;
		}
		else if (pagingBuffer->GetSelectionBeginOffset() < 0)
		{
			nID = ID_COMMAND_FINDNEXT;
		}
		else
		{
			nID = ID_COMMAND_REPLACE;
		}
	}
	else if (findReplaceOption != searchedOption)
	{
		nID = ID_COMMAND_FIND;
	}
	else if (findReplaceOption == searchedOption)
	{
		nID = ID_COMMAND_FINDNEXT;
	}

	command = commandFactory.Create(parent, nID, findReplaceDialog);

	return command;
}