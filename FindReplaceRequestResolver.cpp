#include <afxwin.h>
#include "FindReplaceRequestResolver.h"
#include "NotepadForm.h"
#include "SearchResultController.h"
#include "PagingBuffer.h"
#include "FindReplaceOptionMaker.h"

#pragma warning(disable:4996)

FindReplaceRequestResolver::FindReplaceRequestResolver(CWnd *parent) {
	this->parent = parent;
}

FindReplaceRequestResolver::~FindReplaceRequestResolver() {

}

void FindReplaceRequestResolver::SupplementOption(FindReplaceOption& findReplaceOption) {
	CFindReplaceDialog* findReplaceDialog = ((NotepadForm*)(this->parent))->findReplaceDialog;
	if (findReplaceOption == FindReplaceOption() && findReplaceDialog != NULL)
	{
		findReplaceOption = FindReplaceOptionMaker::Make(findReplaceDialog);
	}
}

AppID FindReplaceRequestResolver::ResolveAppID(AppID appID, const FindReplaceOption findReplaceOption) {
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption previousOption = searchResultController->GetFindReplaceOption();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

	AppID nID = appID;
	switch (appID)
	{
	case AppID::ID_FINDREPLACE_FIND_NEXT_BUTTON:
		if (previousOption != findReplaceOption)
		{
			nID = AppID::ID_ACTION_FIND;
		}
		else
		{
			nID = AppID::ID_ACTION_FIND_NEXT;
		}
		break;
	case AppID::ID_FINDREPLACE_REPLACE_CURRENT_BUTTON:
		if (previousOption != findReplaceOption)
		{
			nID = AppID::ID_ACTION_FIND;
		}
		else if (selectionBeginOffset < 0)
		{
			nID = AppID::ID_ACTION_FIND_NEXT;
		}
		else
		{
			nID = AppID::ID_COMMAND_REPLACE;
		}
		break;
	case AppID::ID_FINDREPLACE_REPLACE_ALL_BUTTON:
		nID = AppID::ID_COMMAND_REPLACE_ALL;
		break;
	case AppID::ID_FINDREPLACE_CLOSE_BUTTON:
		nID = AppID::ID_ACTION_CLOSE_FIND_REPLACE_DIALOG;
		break;
	default:
		break;
	}

	return nID;
}

bool FindReplaceRequestResolver::IsCommand(AppID appID) {
	bool isCommand = false;

	switch (appID)
	{
	case AppID::ID_COMMAND_REPLACE:
	case AppID::ID_COMMAND_REPLACE_ALL:
		isCommand = true;
		break;
	default:
		break;
	}

	return isCommand;
}
