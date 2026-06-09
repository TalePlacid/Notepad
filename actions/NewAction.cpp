#include <afxdlgs.h>
#include "NewAction.h"
#include "../NotepadForm.h"
#include "../glyphs/GlyphFactory.h"
#include "../glyphs/Glyph.h"
#include "../glyphs/NoteWidthCache.h"
#include "../PagingBuffer.h"
#include "../HistoryBook.h"
#include "../CaptionController.h"

#pragma warning(disable:4996)

NewAction::NewAction(CWnd* parent)
	:Action(parent) {

}

NewAction::~NewAction() {

}

void NewAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	if (notepadForm->note != NULL)
	{
		delete notepadForm->note;
		notepadForm->note = NULL;
	}

	char character = '\0';
	notepadForm->note = GlyphFactory::Create(&character);
	character = '\r';
	notepadForm->note->Add(GlyphFactory::Create(&character));
	
	if (notepadForm->noteWidthCache != NULL)
	{
		delete notepadForm->noteWidthCache;
		notepadForm->noteWidthCache = NULL;
	}
	notepadForm->noteWidthCache = new NoteWidthCache(notepadForm, notepadForm->note);

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Clear();
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(-rowStartIndex);

	notepadForm->undoHistoryBook->Clear();
	notepadForm->redoHistoryBook->Clear();

	notepadForm->AssignSourcePath(CString(""));
	notepadForm->ApplyEncoding(ANSI);

	notepadForm->GetParent()->SetWindowTextA("메모장 ~제목없음");
	notepadForm->captionController->UpdateCaption();
}

