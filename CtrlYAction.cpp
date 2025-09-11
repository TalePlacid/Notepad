#include "CtrlYAction.h"
#include "NotepadForm.h"
#include "HistoryBook.h"

#pragma warning(disable:4996)

CtrlYAction::CtrlYAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlYAction::~CtrlYAction() {

}

void CtrlYAction::Perform() {
	HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
	if (!redoHistoryBook->IsEmpty())
	{
		//History history = redoHistoryBook->Pop();
		//history.Execute();

		HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
		//undoHistoryBook->Push(history);

		((NotepadForm*)(this->parent))->Notify("CreateScrollBars");
		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		((NotepadForm*)(this->parent))->Notify("ChangeCaret");
		this->parent->Invalidate();
	}
}