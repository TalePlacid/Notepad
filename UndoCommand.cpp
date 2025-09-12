#include <afxwin.h>
#include "UndoCommand.h"
#include "NotepadForm.h"
#include "HistoryBook.h"

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
		((NotepadForm*)(this->parent))->redoHistoryBook->Push(history);
	}

	((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->Notify("ChangeCaret");
	this->parent->Invalidate();
}