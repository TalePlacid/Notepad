#include <afxwin.h>
#include "RedoCommand.h"
#include "NotepadForm.h"
#include "HistoryBook.h"
#include "resource.h"

#pragma warning(disable:4996)

RedoCommand::RedoCommand(CWnd* parent)
	:Command(parent) {

}

RedoCommand::~RedoCommand() {

}

void RedoCommand::Execute() {
	HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
	if (!redoHistoryBook->IsEmpty())
	{
		Command* history = redoHistoryBook->Pop();
		history->Execute();

		HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
		HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
		if (history->GetId() == ID_COMMAND_REPLACE)
		{
			Long difference = history->GetReplaced().GetLength() - history->GetSource().GetLength();
			undoHistoryBook->Update(history, difference);
			redoHistoryBook->Update(history, difference);
		}
		undoHistoryBook->Push(history);

		((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		((NotepadForm*)(this->parent))->Notify("ChangeCaret");
		this->parent->Invalidate();
	}
}