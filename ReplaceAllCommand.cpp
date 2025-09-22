#include <afxwin.h>
#include <afxdlgs.h>
#include "ReplaceAllCommand.h"
#include "Command.h"
#include "NotepadForm.h"
#include "SearchResultController.h"
#include "PagingBuffer.h"
#include "HistoryBook.h"
#include "MacroCommand.h"
#include "CommandFactory.h"
#include "resource.h"

#pragma warning(disable:4996)

ReplaceAllCommand::ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:MacroCommand(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

ReplaceAllCommand::~ReplaceAllCommand() {

}

void ReplaceAllCommand::Execute() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->FirstRow();

	if (((NotepadForm*)(this->parent))->searchResultController != NULL)
	{
		delete ((NotepadForm*)(this->parent))->searchResultController;
		((NotepadForm*)(this->parent))->searchResultController = NULL;
	}
	((NotepadForm*)(this->parent))->searchResultController = new SearchResultController;

	CommandFactory commandFactory;
	Command* command = commandFactory.Create(this->parent, ID_COMMAND_FIND, this->findReplaceDialog);
	if (command != NULL)
	{
		command->Execute();
		delete command;
		command = NULL;
	}

	HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
	HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	TRACE("[Execute]\n");
	Long i = 0;
	while (i < searchResultController->GetLength())
	{
		command = commandFactory.Create(this->parent, ID_COMMAND_REPLACE, this->findReplaceDialog);
		if (command != NULL)
		{
			command->Execute();
			TRACE("%ld\n", command->GetOffset());
			undoHistoryBook->Update(command, true);
			redoHistoryBook->Update(command, true);
			this->Add(command);
			command = NULL;
		}

		command = commandFactory.Create(this->parent, ID_COMMAND_FINDNEXT, this->findReplaceDialog);
		if (command != NULL)
		{
			command->Execute();
			delete command;
			command = NULL;
		}

		i++;
	}
	
	CString message;
	message.Format("%ld개를 바꾸었습니다.", this->length);
	this->parent->MessageBox(message);
}

void ReplaceAllCommand::Undo() {
	TRACE("[Undo]\n");
	Long j;
	Long i = this->length - 1;
	while (i >= 0)
	{
		this->commands[i]->Undo();
		TRACE("%ld\n", this->commands[i]->GetOffset());
		j = i + 1;
		while (j < this->length)
		{
			this->commands[j]->Update(this->commands[i], false);
			TRACE("\t%d, %ld\n", j, this->commands[j]->GetOffset());
			j++;
		}
		i--;
	}
}

void ReplaceAllCommand::Redo() {
	TRACE("[Redo]\n");
	HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
	HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
	Command* command;
	Long j;
	Long i = 0;
	while (i < this->length)
	{
		command = this->commands.GetAt(i);
		command->Redo();
		TRACE("%ld\n", command->GetOffset());
		j = i + 1;
		while (j < this->length)
		{
			this->commands[j]->Update(command, true);
			j++;
		}
;		undoHistoryBook->Update(command, true);
		redoHistoryBook->Update(command, true);
		i++;
	}
}

void ReplaceAllCommand::Update(Command* command, bool isDone) {
	Long i = 0;
	while (i < this->length && this->commands[i]->GetOffset() > command->GetOffset())
	{
		this->commands[i]->Update(command, isDone);
		i++;
	}
}

bool ReplaceAllCommand::IsUndoable() {
	return true;
}

CString ReplaceAllCommand::GetSource() {
	return this->commands[0]->GetSource();
}

CString ReplaceAllCommand::GetReplaced() {
	return this->commands[0]->GetReplaced();
}