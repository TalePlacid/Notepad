#include <afxdlgs.h>
#include "resource.h"
#include "CommandFactory.h"
#include "SelectFontCommand.h"
#include "OpenCommand.h"
#include "SaveCommand.h"
#include "NewCommand.h"
#include "EraseCommand.h"
#include "AutoWrapCommand.h"
#include "PasteCommand.h"
#include "EraseRangeCommand.h"
#include "CopyCommand.h"
#include "CutCommand.h"
#include "OpenFindDialogCommand.h"
#include "OpenReplaceDialogCommand.h"
#include "UndoCommand.h"
#include "RedoCommand.h"
#include "ReplaceCommand.h"
#include "ReplaceAllCommand.h"
#include "CloseFindReplaceCommand.h"
#include "FindCommand.h"
#include "FindNextCommand.h"
#include "PreviewCommand.h"
#include "LoadNextCommand.h"
#include "LoadPreviousCommand.h"
#include "LoadLastCommand.h"
#include "LoadFirstCommand.h"
#include "SetPageCommand.h"

#pragma warning(disable:4996)

CommandFactory::CommandFactory() {

}

CommandFactory::~CommandFactory() {

}

Command* CommandFactory::Create(CWnd* parent, UINT nID, LPARAM lParam) {
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
	case ID_MENU_SETPAGE:
		command = new SetPageCommand(parent);
		break;
	case ID_MENU_PREVIEW:
		command = new PreviewCommand(parent);
		break;	
	case ID_MENU_FONT:
		command = new SelectFontCommand(parent);
		break;
	case ID_MENU_AUTOWRAP:
	case ID_COMMAND_AUTOWRAP:
		command = new AutoWrapCommand(parent);
		break;
	case ID_MENU_FIND:
		command = new OpenFindDialogCommand(parent);
		break;
	case ID_MENU_REPLACE:
		command = new OpenReplaceDialogCommand(parent);
		break;
	case ID_COMMAND_ERASE:
		command = new EraseCommand(parent);
		break;
	case ID_COMMAND_PASTE:
		command = new PasteCommand(parent);
		break;
	case ID_COMMAND_ERASERANGE:
		command = new EraseRangeCommand(parent);
		break;
	case ID_COMMAND_COPY:
		command = new CopyCommand(parent);
		break;
	case ID_COMMAND_CUT:
		command = new CutCommand(parent);
		break;
	case ID_COMMAND_UNDO:
		command = new UndoCommand(parent);
		break;
	case ID_COMMAND_REDO:
		command = new RedoCommand(parent);
		break;
	case ID_COMMAND_REPLACE:
		command = new ReplaceCommand(parent, (CFindReplaceDialog*)lParam);
		break;
	case ID_COMMAND_CLOSEFINDREPLACE:
		command = new CloseFindReplaceCommand(parent, (CFindReplaceDialog*)lParam);
		break;
	case ID_COMMAND_REPLACEALL:
		command = new ReplaceAllCommand(parent, (CFindReplaceDialog*)lParam);
		break;
	case ID_COMMAND_FIND:
		command = new FindCommand(parent, (CFindReplaceDialog*)lParam);
		break;
	case ID_COMMAND_FINDNEXT:
		command = new FindNextCommand(parent, (CFindReplaceDialog*)lParam);
		break;
	case ID_COMMAND_LOADFIRST:
		command = new LoadFirstCommand(parent);
		break;	
	case ID_COMMAND_LOADPREVIOUS:
		command = new LoadPreviousCommand(parent);
		break;
	case ID_COMMAND_LOADNEXT:
		command = new LoadNextCommand(parent);
		break;
	case ID_COMMAND_LOADLAST:
		command = new LoadLastCommand(parent);
		break;

	default:
		break;
	}

	return command;
}
