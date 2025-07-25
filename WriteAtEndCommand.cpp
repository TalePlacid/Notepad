#include "WriteAtEndCommand.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

WriteAtEndCommand::WriteAtEndCommand(CWnd* parent, char(*character))
	:Command(parent) {
	this->character[0] = character[0];
	this->character[1] = character[1];
}

WriteAtEndCommand::~WriteAtEndCommand() {

}

WriteAtEndCommand::WriteAtEndCommand(const WriteAtEndCommand& source)
	:Command(parent) {
	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
}

WriteAtEndCommand& WriteAtEndCommand::operator=(const WriteAtEndCommand& source) {
	Command::operator=(source);

	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];

	return *this;
}

void WriteAtEndCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Glyph* row = note->GetAt(note->GetCurrent());
	
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove();
		pagingBuffer->Remove();
	}

	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		pagingBuffer->Add((char*)(*glyph));
	}
	else
	{
		note->Add(glyph);
		TCHAR contents[2];
		contents[0] = '\r';
		contents[1] = '\n';
		pagingBuffer->Add(contents);
	}

	pagingBuffer->MarkAsDirty();
}

void WriteAtEndCommand::Unexecute() {

}