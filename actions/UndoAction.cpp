#include <afxwin.h>
#include "UndoAction.h"
#include "../NotepadForm.h"
#include "../HistoryBook.h"
#include "../commands/Command.h"

#pragma warning(disable:4996)

UndoAction::UndoAction(CWnd* parent)
	:Action(parent) {

}

UndoAction::~UndoAction() {

}

void UndoAction::Perform() {
	HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
	if (!undoHistoryBook->IsEmpty())
	{
		Command* history = undoHistoryBook->Pop();
		history->Undo();

		HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
		redoHistoryBook->Push(history);

		((NotepadForm*)(this->parent))->isDirty = TRUE;
	}
}