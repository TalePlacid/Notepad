#include "FindReplaceCommandFactory.h"
#include "Command.h"
#include "FindCommand.h"
#include "FindNextCommand.h"
#include "resource.h"

#pragma warning(disable:4996)

FindReplaceCommandFactory::FindReplaceCommandFactory() {

}

FindReplaceCommandFactory::~FindReplaceCommandFactory() {

}

Command* FindReplaceCommandFactory::Create(CWnd* parent, CFindReplaceDialog* findReplaceDialog, UINT id) {
	Command* command = NULL;

	switch (id)
	{
	case ID_COMMAND_FIND:
		command = new FindCommand(parent, findReplaceDialog);
		break;
	case ID_COMMAND_FINDNEXT:
		command = new FindNextCommand(parent, findReplaceDialog);
		break;
	default:
		break;
	}

	return command;
}