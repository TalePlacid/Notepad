#include <afxwin.h>
#include "UndoCommand.h"
#include "NotepadForm.h"
#include "HistoryBook.h"
#include "resource.h"

#pragma warning(disable:4996)

UndoCommand::UndoCommand(CWnd* parent)
	:Command(parent) {

}

UndoCommand::~UndoCommand() {

}

void UndoCommand::Execute() {
	HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
	if (!undoHistoryBook->IsEmpty())
	{
		Command* history = undoHistoryBook->Pop();
		history->Undo();
		HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
		HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
		if (history->GetId() == ID_COMMAND_REPLACE)
		{
			Long difference = history->GetSource().GetLength() - history->GetReplaced().GetLength();
			undoHistoryBook->Update(history, difference);
			redoHistoryBook->Update(history, difference);
		}
		redoHistoryBook->Push(history);
	}

	((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->Notify("ChangeCaret");
	this->parent->Invalidate();
}