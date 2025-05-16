#include "WriteAtEndCommand.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "NotepadForm.h"

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

	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(((NotepadForm*)(this->parent))->note->GetCurrent());
	
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(row->GetLength()-1);
	}

	if (this->character[0] != '\r')
	{
		row->Add(glyph);
	}
	else
	{
		((NotepadForm*)(this->parent))->note->Add(glyph);
	}
}

void WriteAtEndCommand::Unexecute() {

}