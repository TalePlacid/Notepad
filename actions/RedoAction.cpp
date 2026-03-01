#include <afxwin.h>
#include "RedoAction.h"
#include "../NotepadForm.h"
#include "../HistoryBook.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../commands/Command.h"

#pragma warning(disable:4996)

RedoAction::RedoAction(CWnd* parent)
	:Action(parent) {

}

RedoAction::~RedoAction() {

}

void RedoAction::Perform() {
	((NotepadForm*)(this->parent))->note->Select(false);
	((NotepadForm*)(this->parent))->pagingBuffer->UnmarkSelectionBegin();

	HistoryBook* redoHistoryBook = ((NotepadForm*)(this->parent))->redoHistoryBook;
	if (!redoHistoryBook->IsEmpty())
	{
		Command* history = redoHistoryBook->Pop();
		history->Redo();

		HistoryBook* undoHistoryBook = ((NotepadForm*)(this->parent))->undoHistoryBook;
		undoHistoryBook->Push(history);

		((NotepadForm*)(this->parent))->isDirty = TRUE;
	}
}
