#include "resource.h"
#include "CommandFactory.h"
#include "SelectFontCommand.h"
#include "OpenCommand.h"
#include "SaveCommand.h"
#include "NewCommand.h"
#include "EraseCommand.h"
#include "AutoWrapCommand.h"

#pragma warning(disable:4996)

CommandFactory::CommandFactory() {

}

CommandFactory::~CommandFactory() {

}

Command* CommandFactory::Create(CWnd* parent, UINT nID) {
	Command* command = NULL;

	switch (nID)
	{
	case ID_MENU_NEW:
		command = new NewCommand(parent);
		break;
	case ID_MENU_OPEN:
		command = new OpenCommand(parent);
		break;
	case ID_MENU_SAVE:
		command = new SaveCommand(parent);
		break;
	case ID_MENU_SAVEAS:
		command = new SaveCommand(parent);
		break;
	case ID_MENU_FONT:
		command = new SelectFontCommand(parent);
		break;
	case ID_MENU_AUTOWRAP:
		command = new AutoWrapCommand(parent);
		break;
	case ID_COMMAND_ERASE:
		command = new EraseCommand(parent);
		break;
	default:
		break;
	}

	return command;
}
