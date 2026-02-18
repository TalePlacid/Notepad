#include <afxdlgs.h>
#include "NewAction.h"
#include "../NotepadForm.h"
#include "../glyphs/GlyphFactory.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"

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

	GlyphFactory glyphFactory;
	char character = '\0';
	notepadForm->note = glyphFactory.Create(&character);
	character = '\r';
	notepadForm->note->Add(glyphFactory.Create(&character));

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Clear();
	Long rowStartIndex = pagingBuffer->GetRowStartIndex();
	pagingBuffer->CacheRowStartIndex(-rowStartIndex);

	notepadForm->sourcePath = CString("");

	notepadForm->parent->SetWindowTextA("메모장 ~제목없음");

	this->parent->Invalidate();
}