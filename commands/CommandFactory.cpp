#include <afxdlgs.h>
#include "../AppID.h"
#include "CommandFactory.h"
#include "WriteAtEndCommand.h"
#include "InsertAtCaretCommand.h"
#include "EraseCommand.h"
#include "PasteCommand.h"
#include "EraseRangeCommand.h"
#include "CutCommand.h"
#include "OpenFindDialogCommand.h"
#include "OpenReplaceDialogCommand.h"
#include "UndoCommand.h"
#include "RedoCommand.h"
#include "PreviewCommand.h"
#include "SetPageCommand.h"
#include "PrintCommand.h"
#include "ZoomInCommand.h"
#include "ZoomOutCommand.h"
#include "ZoomResetCommand.h"
#include "../findReplaces/ReplaceCommand.h"
#include "../findReplaces/ReplaceAllCommand.h"
#include "../findReplaces/CloseFindReplaceCommand.h"
#include "../findReplaces/FindCommand.h"
#include "../findReplaces/FindNextCommand.h"

#pragma warning(disable:4996)

CommandFactory::CommandFactory() {

}

CommandFactory::~CommandFactory() {

}

Command* CommandFactory::Create(CWnd* parent, AppID nID, const TCHAR(*character), 
	BOOL isCompositing, BOOL isSelected, LPARAM lParam) {
	Command* command = NULL;

	switch (nID)
	{
	case AppID::ID_COMMAND_WRITE_AT_END:
		command = new WriteAtEndCommand(parent, character, isCompositing);
		break;
	case AppID::ID_COMMAND_INSERT_AT_CARET:
		command = new InsertAtCaretCommand(parent, character, isCompositing);
		break;
	case AppID::ID_COMMAND_ERASE:
		if (isSelected)
		{
			command = new EraseRangeCommand(parent);
		}
		else
		{
			command = new EraseCommand(parent, isCompositing);
		}
		break;
	case AppID::ID_COMMAND_PASTE:
		if (isSelected)
		{
			command = new MacroCommand(parent, 2);
			command->Add(new EraseRangeCommand(parent));
			command->Add(new PasteCommand(parent));
		}
		else
		{
			command = new PasteCommand(parent);
		}
		break;
#if 0
	case ID_MENU_NEW:
		command = new NewCommand(parent);
		break;
	case ID_MENU_NEWWINDOW:
		command = new NewWindowCommand(parent);
		break;
	case ID_MENU_OPEN:
		command = new OpenCommand(parent);
		break;
	case ID_MENU_SAVE:
		command = new SaveCommand(parent);
		break;
	case ID_MENU_SAVEAS:
		command = new SaveAsCommand(parent);
		break;
	case ID_MENU_SETPAGE:
		command = new SetPageCommand(parent);
		break;
	case ID_MENU_PREVIEW:
		command = new PreviewCommand(parent);
		break;	
	case ID_MENU_PRINT:
		command = new PrintCommand(parent);
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
	case ID_MENU_ZOOMIN:
		command = new ZoomInCommand(parent);
		break;
	case ID_MENU_ZOOMOUT:
		command = new ZoomOutCommand(parent);
		break;
	case ID_MENU_ZOOMRESET:
		command = new ZoomResetCommand(parent);
		break;
	case ID_COMMAND_WRITEATEND:
		command = new WriteAtEndCommand(parent, character, onChar);
		break;
	case ID_COMMAND_INSERTATCARET:
		command = new InsertAtCaretCommand(parent, character, onChar);
		break;	
	case ID_COMMAND_ERASE:
		command = new EraseCommand(parent);
		break;
	case ID_MENU_PASTE:
	case ID_COMMAND_PASTE:
		command = new PasteCommand(parent);
		break;
	case ID_MENU_DELETE:
	case ID_COMMAND_ERASERANGE:
		command = new EraseRangeCommand(parent);
		break;
	case ID_MENU_COPY:
	case ID_COMMAND_COPY:
		command = new CopyCommand(parent);
		break;
	case ID_MENU_CUT:
	case ID_COMMAND_CUT:
		command = new CutCommand(parent);
		break;
	case ID_MENU_UNDO:
	case ID_COMMAND_UNDO:
		command = new UndoCommand(parent);
		break;
	case ID_MENU_REDO:
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
#endif
	default:
		break;
	}

	return command;
}
