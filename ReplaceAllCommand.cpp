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

#pragma warning(disable:4996)

ReplaceAllCommand::ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:FindReplaceCommand(parent, findReplaceDialog) {

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
	Long count = 0;
	Long i = 0;
	while (i < searchResultController->GetLength())
	{
		command = new ReplaceCommand(this->parent, this->findReplaceDialog);
		if (command != NULL)
		{
			command->Execute();
			command = NULL;
		}

		command = new FindNextCommand(this->parent, this->findReplaceDialog);
		if (command != NULL)
		{
			command->Execute();
			command = NULL;
		}

		count++;
		i++;
	}

	CString message;
	message.Format("%ld개를 바꾸었습니다.", count);
	this->parent->MessageBox(message);
}