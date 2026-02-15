#include <afxwin.h>
#include <afxdlgs.h>
#include "ReplaceAllCommand.h"
#include "../resource.h"
#include "FindReplaceOption.h"
#include "../NotepadForm.h"
#include "../SearchResultController.h"
#include "../PagingBuffer.h"
#include "../HistoryBook.h"
#include "../commands/Command.h"
#include "../commands/MacroCommand.h"
#include "../commands/CommandFactory.h"

#pragma warning(disable:4996)

ReplaceAllCommand::ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:MacroCommand(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

ReplaceAllCommand::~ReplaceAllCommand() {

}

void ReplaceAllCommand::Execute() {
	//1. 전체 문서의 첫 위치로 이동한다.
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);

	//2. 찾는다.
	CommandFactory commandFactory;
	Command* command = commandFactory.Create(this->parent, ID_COMMAND_FIND, 0, FALSE, (LPARAM)(this->findReplaceDialog));
	if (command != NULL)
	{
		command->Execute();
		delete command;
		command = NULL;
	}

	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	Long i = 0;
	Long count = searchResultController->GetLength();
	while (i < count)
	{
		command = commandFactory.Create(this->parent, ID_COMMAND_REPLACE, 0, FALSE, (LPARAM)(this->findReplaceDialog));
		if (command != NULL)
		{
			command->Execute();
			this->Add(command);
			command = NULL;
		}
		i++;
	}
}

void ReplaceAllCommand::Undo() {
	Long i = this->length - 1;
	while (i >= 0)
	{
		this->commands[i]->Undo();
		i--;
	}
}

void ReplaceAllCommand::Redo() {
	Command* command;
	Long i = 0;
	while (i < this->length)
	{
		command = this->commands.GetAt(i);
		command->Redo();
		i++;
	}
}

bool ReplaceAllCommand::IsUndoable() {
	bool ret = false;
	if (this->length > 0)
	{
		ret = true;
	}

	return ret;
}

CString ReplaceAllCommand::GetSource() {
	return this->commands[0]->GetSource();
}

CString ReplaceAllCommand::GetReplaced() {
	return this->commands[0]->GetReplaced();
}

FindReplaceOption* ReplaceAllCommand::GetFindReplaceOption() {
	return this->commands[0]->GetFindReplaceOption();
}