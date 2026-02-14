#include <afxwin.h>
#include "RedoCommand.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../HistoryBook.h"

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
		history->Redo();

		HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
		undoHistoryBook->Push(history);

		((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		((NotepadForm*)(this->parent))->Notify("ChangeCaret");
		this->parent->Invalidate();
	}
}