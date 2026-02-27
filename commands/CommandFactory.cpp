#include <afxdlgs.h>
#include "../AppID.h"
#include "../FindReplaceOption.h"
#include "CommandFactory.h"
#include "WriteAtEndCommand.h"
#include "InsertAtCaretCommand.h"
#include "EraseCommand.h"
#include "PasteCommand.h"
#include "EraseRangeCommand.h"
#include "CutCommand.h"
#include "ReplaceCommand.h"
#include "ReplaceAllCommand.h"

#pragma warning(disable:4996)

CommandFactory::CommandFactory() {

}

CommandFactory::~CommandFactory() {

}

Command* CommandFactory::Create(CWnd* parent, AppID nID, const TCHAR(*character), 
	BOOL onChar, BOOL isSelected, FindReplaceOption* findReplaceOption) {
	Command* command = NULL;

	switch (nID)
	{
	case AppID::ID_COMMAND_WRITE_AT_END:
		command = new WriteAtEndCommand(parent, character, onChar);
		break;
	case AppID::ID_COMMAND_INSERT_AT_CARET:
		command = new InsertAtCaretCommand(parent, character, onChar);
		break;
	case AppID::ID_COMMAND_ERASE:
		if (isSelected)
		{
			command = new EraseRangeCommand(parent);
		}
		else
		{
			command = new EraseCommand(parent, onChar);
		}
		break;
	case AppID::ID_COMMAND_PASTE:
		command = new PasteCommand(parent);
		break;
	case AppID::ID_COMMAND_CUT:
		command = new CutCommand(parent);
		break;
	case AppID::ID_COMMAND_REPLACE:
		command = new ReplaceCommand(parent, *findReplaceOption);
		break;
	case AppID::ID_COMMAND_REPLACE_ALL:
		command = new ReplaceAllCommand(parent, *findReplaceOption);
		break;
	default:
		break;
	}

	return command;
}
