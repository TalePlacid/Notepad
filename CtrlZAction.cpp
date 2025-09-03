#include <afxwin.h>
#include "CtrlZAction.h"
#include "NotepadForm.h"
#include "HistoryBook.h"
#include "History.h"
#include "HistoryBinder.h"

#pragma warning(disable:4996)

CtrlZAction::CtrlZAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlZAction::~CtrlZAction() {

}

void CtrlZAction::Perform() {
	HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
	HistoryBinder* historyBinder = ((NotepadForm*)(this->parent))->historyBinder;
	if (historyBinder->GetLength() > 0)
	{
		History history = historyBinder->Commit();
		undoHistoryBook->Push(history);
	}

	if (!undoHistoryBook->IsEmpty())
	{
		History history = undoHistoryBook->Pop();
		history.Undo();
		((NotepadForm*)(this->parent))->redoHistoryBook->Push(history);
	}

	((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->Notify("ChangeCaret");
	this->parent->Invalidate();

}

