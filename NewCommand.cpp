#include "NewCommand.h"
#include "NotepadForm.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

NewCommand::NewCommand(CWnd* parent)
	:Command(parent) {

}

NewCommand::~NewCommand() {

}

void NewCommand::Execute() {
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

	TCHAR buffer[256];
	GetCurrentDirectory(256, buffer);
	CString path(buffer);
	notepadForm->path = path + CString("\\NoName.txt");

	notepadForm->parent->SetWindowTextA("메모장 ~제목없음");

	this->parent->Invalidate();
}
