#include <afxdlgs.h>
#include "FindReplaceInterpreter.h"

#pragma warning(disable:4996)

FindReplaceInterpreter::FindReplaceInterpreter() {

}

FindReplaceInterpreter::~FindReplaceInterpreter() {

}

AppID FindReplaceInterpreter::DetermineID(LPARAM lParam) {
	CFindReplaceDialog* findReplaceDialog = CFindReplaceDialog::GetNotifier(lParam);

	AppID appID;
	if (findReplaceDialog->IsTerminating())
	{
		appID = AppID::ID_ACTION_CLOSE_FIND_REPLACE_DIALOG;
	}
	else if (findReplaceDialog->FindNext())
	{
		appID = AppID::ID_ACTION_FIND_NEXT;
	}
	else if (findReplaceDialog->ReplaceCurrent())
	{
		appID = AppID::ID_COMMAND_REPLACE;
	}
	else if (findReplaceDialog->ReplaceAll())
	{
		appID = AppID::ID_COMMAND_REPLACE_ALL;
	}
	else
	{
		appID = AppID::ID_ACTION_FIND;
	}

	return appID;
}

BOOL FindReplaceInterpreter::IsCommand(AppID appID) {
	BOOL isCommand = FALSE;

	switch (appID)
	{
	case AppID::ID_COMMAND_REPLACE:
	case AppID::ID_COMMAND_REPLACE_ALL:
		isCommand = TRUE;
	default:
		break;
	}

	return isCommand;
}