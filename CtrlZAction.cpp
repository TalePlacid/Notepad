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
	HistoryBook* historyBook = ((NotepadForm*)(this->parent))->historyBook;
	HistoryBinder* historyBinder = ((NotepadForm*)(this->parent))->historyBinder;
	if (historyBinder->GetLength() > 0)
	{
		History history = historyBinder->Commit();
		historyBook->Push(history);
	}

	if (!historyBook->IsEmpty())
	{
		History history = historyBook->Pop();
		history.Undo();
	}

	((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	((NotepadForm*)(this->parent))->Notify("ChangeCaret");
	this->parent->Invalidate();

}

