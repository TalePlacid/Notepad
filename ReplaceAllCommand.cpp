#include <afxwin.h>
#include <afxdlgs.h>
#include "ReplaceAllCommand.h"
#include "Command.h"
#include "FindCommand.h"
#include "FindNextCommand.h"
#include "ReplaceCommand.h"
#include "NotepadForm.h"
#include "SearchResultController.h"
#include "PagingBuffer.h"
#include "HistoryBook.h"
#include "MacroCommand.h"

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

	Command* command = new FindCommand(this->parent, this->findReplaceDialog);
	if (command != NULL)
	{
		command->Execute();
		delete command;
		command = NULL;
	}

	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	Command* history = new MacroCommand(this->parent);
	Long i = 0;
	while (i < searchResultController->GetLength())
	{
		command = new ReplaceCommand(this->parent, this->findReplaceDialog);
		if (command != NULL)
		{
			command->Execute();
			history->Add(command);
			command = NULL;
		}

		command = new FindNextCommand(this->parent, this->findReplaceDialog);
		if (command != NULL)
		{
			command->Execute();
			delete command;
			command = NULL;
		}

		i++;
	}

	HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
	HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
	Long difference = history->GetReplaced().GetLength() - history->GetSource().GetLength();

#if 0
	i = 0;
	while (i < this->length)
	{
		findCommand = this->commands[i]
		undoHistoryBook->Update(findCommand, difference);
		redoHistoryBook->Update(findCommand, difference);
		i++;
	}
#endif
	undoHistoryBook->Update(history, difference);
	redoHistoryBook->Update(history, difference);
	undoHistoryBook->Push(history);

	CString message;
	message.Format("%ld개를 바꾸었습니다.", history->GetLength());
	this->parent->MessageBox(message);
}