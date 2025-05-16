#include "NewCommand.h"
#include "NotepadForm.h"
#include "GlyphFactory.h"

#pragma warning(disable:4996)

NewCommand::NewCommand(CWnd* parent)
	:Command(parent) {

}

NewCommand::~NewCommand() {

}

NewCommand::NewCommand(const NewCommand& source)
	:Command(source.parent) {

}

NewCommand& NewCommand::operator=(const NewCommand& source) {
	Command::operator=(source);

	return *this;
}

void NewCommand::Execute() {
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
		((NotepadForm*)(this->parent))->note = NULL;
	}

	GlyphFactory glyphFactory;
	char character = '\0';
	((NotepadForm*)(this->parent))->note = glyphFactory.Create(&character);

	this->parent->Invalidate();
}

void NewCommand::Unexecute() {

}