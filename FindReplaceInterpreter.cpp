#include <afxdlgs.h>
#include "FindReplaceInterpreter.h"

#pragma warning(disable:4996)

FindReplaceInterpreter::FindReplaceInterpreter() {

}

FindReplaceInterpreter::~FindReplaceInterpreter() {

}

AppID FindReplaceInterpreter::DetermineID(CFindReplaceDialog* findReplaceDialog) {
	AppID rawID = AppID::NONE;
	if (findReplaceDialog->IsTerminating())
	{
		rawID = AppID::ID_FINDREPLACE_CLOSE_BUTTON;
	}
	else if (findReplaceDialog->FindNext())
	{
		rawID = AppID::ID_FINDREPLACE_FIND_NEXT_BUTTON;
	}
	else if (findReplaceDialog->ReplaceCurrent())
	{
		rawID = AppID::ID_FINDREPLACE_REPLACE_CURRENT_BUTTON;
	}
	else if (findReplaceDialog->ReplaceAll())
	{
		rawID = AppID::ID_FINDREPLACE_REPLACE_ALL_BUTTON;
	}

	return rawID;
}
